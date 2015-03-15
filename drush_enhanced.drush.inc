<?php
/**
 * Implements hook_drush_command
 */
function drush_enhanced_drush_command() {
  $options['database'] = array(
    'description' => 'The DB connection key if using multiple connections in settings.php',
    'example-value' => 'extra',
  );
  $options['target'] = array(
    'description' => 'The name of a target within the specified database.',
    'example-value' => 'key',
    'hidden' => TRUE,
  );
  $db_url['db-url'] = array(
    'description' => 'A Drupal 6 style database URL. Only required for initial install - not re-install.',
    'example-value' => 'mysql://root:pass@127.0.0.1/db',
  );

  //Definde the drupal enhanced items
  $items['sql-clean-sync'] = array(
    'description' => 'Copy and import source database to target database. But first drop the target database. Transfer via rsync.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'drush dependencies' => array('core', 'sql'), // core-rsync, sql-sync
    'examples' => array(
      'drush sql-clean-sync @prod @dev' => 'Copy the DB defined in sites/prod to the DB in sites/dev but first drop the Tables in sites/dev',
    ),
    'argumens' => array(
      'from' => 'Name of subdirectory within /sites or a site-alias.',
      'to' => 'Name of subdirectory within /sites or a site-alias.',
    ),
    'options' => array(
      'skip-tables-key' => 'A key in the $skip_tables array. @see example.drushrc.php. Optional.',
      'skip-tables-list' => 'A comma-separated list of tables to exclude completely. Optional.',
      'structure-tables-key' => 'A key in the $structure_tables array. @see example.drushrc.php. Optional.',
      'structure-tables-list' => 'A comma-separated list of tables to include for structure, but not data. Optional.',
      'tables-key' => 'A key in the $tables array. Optional.',
      'tables-list' => 'A comma-separated list of tables to transfer. Optional.',
      'cache' => 'Skip dump if result file exists and is less than "cache" hours old. Optional; default is 24 hours.',
      'no-cache' => 'Do not cache the sql-dump file.',
      'no-dump' => 'Do not dump the sql database; always use an existing dump file.',
      'source-db-url' => 'Database specification for source system to dump from.',
      'source-remote-port' => 'Override sql database port number in source-db-url. Optional.',
      'source-remote-host' => 'Remote machine to run sql-dump file on. Optional; default is local machine.',
      'source-dump' => 'Path to dump file. Optional; default is to create a temporary file.',
      'source-database' => 'A key in the $db_url (D6) or $databases (D7+) array which provides the data.',
      'source-target' => array(
          'description' => 'A key within the SOURCE database identifying a particular server in the database group.',
          'example-value' => 'key',
          // Gets unhidden in help_alter(). We only want to show to D7 users but have to
          // declare it here since this command does not bootstrap fully.
          'hidden' => TRUE,
      ),
      'target-db-url' => '',
      'target-remote-port' => '',
      'target-remote-host' => '',
      'target-dump' => '',
      'target-database' => 'A key in the $db_url (D6) or $databases (D7+) array which shall receive the data.',
      'target-target' => array(
          'description' => 'Oy. A key within the TARGET database identifying a particular server in the database group.',
          'example-value' => 'key',
          // Gets unhidden in help_alter(). We only want to show to D7 users but have to
          // declare it here since this command does not bootstrap fully.
          'hidden' => TRUE,
      ),
      'temp' => 'Use a temporary file to hold dump files.  Implies --no-cache.',
      'dump-dir' => 'Directory to store sql dump files in when --source-dump or --target-dump are not used.  Takes precedence over --temp.',
      'create-db' => 'Create a new database before importing the database dump on the target machine.',
      'db-su' => array(
        'description' => 'Account to use when creating a new database. Optional.',
        'example-value' => 'root',
      ),
      'db-su-pw' => array(
        'description' => 'Password for the "db-su" account. Optional.',
        'example-value' => 'pass',
      ),
      'no-ordered-dump' => 'Do not pass --ordered-dump to sql-dump.  sql-sync orders the dumpfile by default in order to increase the efficiency of rsync.',
      'sanitize' => 'Obscure email addresses and reset passwords in the user table post-sync. Optional.',
    ),
    'sub-options' => array(
      'sanitize' => array(
        'sanitize-password' => 'The password to assign to all accounts in the sanitization operation, or "no" to keep passwords unchanged.  Default is "password".',
        'sanitize-email' => 'The pattern for test email addresses in the sanitization operation, or "no" to keep email addresses unchanged.  May contain replacement patterns %uid, %mail or %name.  Default is "user+%uid@localhost".',
        'confirm-sanitizations' => 'Prompt yes/no after importing the database, but before running the sanitizations',
      ),
    ),
    'topics' => array('docs-aliases', 'docs-policy', 'docs-example-sync-via-http', 'docs-example-sync-extension'),
  );
  return $items;
}

function drush_drush_enhanced_sql_clean_sync($source = NULL, $destination = NULL) {
  drush_print($source);
  drush_print($destination);
  $args = array(
    'source' => $source,
    'destination' => $destination,
  );
  drush_do_command_redispatch('sql-drop');
  drush_do_command_redispatch('sql-sync', $args);
}
