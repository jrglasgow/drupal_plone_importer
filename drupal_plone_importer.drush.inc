<?php

/**
 * @file
 *
 * take a Plone export in JSON files with a Binaries directory and import into
 * Drupal through drush
 */


/**
 * Implements hook_drush_command().
 */
function drupal_plone_importer_drush_command() {
  $items = array();

  $items['import-from-plone'] = array(
    'description' => 'Import content/files from a Plone Export',
    'arguments' => array(
      'location of plone export' => 'The path to the Plone export, this is a directory with a bunch of .json files and a binaries directory',
    ),
    'examples' => array(
      'drush import-from-plone /path/to/export_files' => 'Process all the JSON files at /path/to/export_files and insert them into Drupal as content and add files to public:// files directory and the files-managed DB table.',
    ),
    'aliases' => array('dpi'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL, // Full bootstrap into Drupal
  );

  $items['plone-import-stats'] = array(
    'description' => 'Read through the Plone export and determine which types are in the export and output the report',
    'arguments' => array(
      'location of plone export' => 'The path to the Plone export, this is a directory with a bunch of .json files and a binaries directory',
    ),
    'examples' => array(
      'drush plone-import-statistics /path/to/export_files' => 'Process all the JSON files at /path/to/export_files and report on the different types available.',
    ),
    'aliases' => array('dpis'),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE, // No bootstrap needed
  );

  return $items;
}

/**
 * Initiate the import of content into Drupal
 */
function drush_import_from_plone($import_base = '') {

}

/**
 * read through the export files and create a list of the types of content
 */
function drush_drupal_plone_importer_plone_import_stats($import_base = '') {
  $dir = opendir($import_base);
  $types = array();
  while ($file = readdir($dir)) {
    $type = array_pop(explode('.', $file));

    switch ($type) {
      case 'json':
        $contents = file_get_contents($import_base . '/' . $file);
        $data = json_decode($contents);
        $types[$data->type]++;
        break;
    }
  }
  if (empty($types)) {
    drush_print('There were no element types discovered.');
    return;
  }
  drush_print(dt('The following element types were found:'));
  asort($types);
  $types = array_reverse($types);
  $rows = array();
  foreach ($types AS $type => $count) {
    $rows[] = array(
      $type,
      $count,
    );
  }
  drush_print_table($rows, FALSE);
}
