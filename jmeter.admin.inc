<?php

function jmeter_landing_page() {
  return 'this is the landing page';
}

function jmeter_configuration_page_form($form, &$form_state) {
  $form = array();

  $form['file_locations'] = array(
    '#type' => 'fieldset',
    '#title' => t('File Locations'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['file_locations']['jmeter_uploads_directory'] = array(
    '#title' => t('Jmeter uploads directory'),
    '#type' => 'textfield',
    '#description' => 'jmeter_uploads/ will be appended to the file path specified. Directory specified should not be web accessible. No trailing slash.',
    '#default_value' => variable_get('jmeter_uploads_directory', 'private://'),
  );

  $form['file_locations']['jmeter_results_directory'] = array(
    '#title' => t('Jmeter results directory'),
    '#type' => 'textfield',
    '#description' => 'jmeter_results/ will be appended to the file path specified. Directory specified should not be web accessible. No trailing slash.',
    '#default_value' => variable_get('jmeter_results_directory', 'private://'),
  );

  $form['troubleshooting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Troubleshooting'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['troubleshooting']['jmeter_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable debug mode',
    '#description' => 'Turning this setting on will produce more messages describing exactly what the code is doing at each step in the process.',
    '#default_value' => variable_get('jmeter_debug_mode', ''),
  );

  return system_configuration_form($form);
}

function jmeter_add_job_form($form, &$form_state) {
  $form = array();

  if (!isset($form_state['stage'])) $form_state['stage'] = 'start';

  switch($form_state['stage']) {
    case 'start':
      return jmeter_add_job_form_start($form, $form_state);
    break;
    case 'details':
      return jmeter_add_job_form_details($form, $form_state);
    break;
  }

  return $form;
}

function jmeter_add_job_form_validate($form, &$form_state) {
  switch($form_state['stage']) {
    case 'start':
      return jmeter_add_job_form_start_validate($form, $form_state);
      break;
    case 'details':
      return jmeter_add_job_form_details_validate($form, $form_state);
      break;
  }
}

function jmeter_add_job_form_start($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => 'Job Description',
    '#description' => 'A descriptive title for this job.',
  );

  $form['continue'] = array(
    '#type' => 'submit',
    '#value' => 'Continue',
  );

  return $form;
}

function jmeter_add_job_form_details($form, &$form_state) {
  $form['heading'] = array(
    '#markup' => '<h2>Setup Job: ' . $form_state['values']['description'] . '</h2>',
  );

  $form['instructions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instructions'),
    '#collapsable' => FALSE,
  );

  $form['instructions']['instruction_text'] = array(
    '#markup' => 'Please upload all necessary files for your job, this includes the jmx file as well as any assets used in the jmx file. Set up your test parameters and then proceed to the review page where you will have one chance to review your parameters before the test starts.',
  );

  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Files'),
    '#collapsable' => TRUE,
    '#collapsed' => FALSE,
  );

  //@TODO - rebuild files section to use ajax to allow unlimited uploads.
  $form['files']['upload1'] = array(
    '#type' => 'managed_file',
    '#title' => 'File #1',
    '#upload_validators' => array('file_validate_extensions' => array('jmx csv')),
  );

  $form['files']['upload2'] = array(
    '#type' => 'managed_file',
    '#title' => 'File #2',
    '#upload_validators' => array('file_validate_extensions' => array('jmx csv')),
  );

  $form['files']['upload3'] = array(
    '#type' => 'managed_file',
    '#title' => 'File #3',
    '#upload_validators' => array('file_validate_extensions' => array('jmx csv')),
  );

  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test Configuration'),
    '#collapsable' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['configuration']['threads'] = array(
    '#type' => 'textfield',
    '#title' => 'Threads',
    '#default_value' => isset($form_state['values']['threads']) ? $form_state['values']['threads'] : '',
    '#requried' => TRUE,
    '#description' => 'Enter the number of threads you wish to run. Make sure \'${__P(threads, 1)}\' is set as the value for the \'Number of Threads\' setting in the \'Thread Group\' section of your Jmeter test.'
  );

  $form['configuration']['duration'] = array(
    '#type' => 'textfield',
    '#title' => 'Duration',
    '#default_value' => isset($form_state['values']['duration']) ? $form_state['values']['duration'] : '',
    '#requried' => TRUE,
    '#description' => 'Enter the number of seconds you want to run the test for. Make sure \'${__P(duration, 1)}\' is set as the value for the \'Duration\' setting in the \'Thread Group\' section of your Jmeter test. Make sure you have the \'Scheduler\' box ticked or duration will not work. Also make sure you have the \'Forever\' box ticked in the loops section.',
  );

  $form['configuration']['or'] = array(
    '#markup' => '-OR-',
  );

  $form['configuration']['loops'] = array(
    '#type' => 'textfield',
    '#title' => 'Loops',
    '#default_value' => isset($form_state['values']['loops']) ? $form_state['values']['loops'] : '',
    '#requried' => TRUE,
    '#description' => 'Enter the number of loops you want the test to run the test for. Make sure \'${__P(loops, 1)}\' is set as the value for the \'Loop Count\' setting in the \'Thread Group\' section of your Jmeter test. Make sure you have the \'Scheduler\' box unchecked as well as the \'Forever\' box.',
  );

  $form['configuration']['warmup'] = array(
    '#type' => 'textfield',
    '#title' => 'Warmup',
    '#default_value' => isset($form_state['values']['warmup']) ? $form_state['values']['warmup'] : '',
    '#requried' => TRUE,
    '#description' => 'Enter the number of seconds you want Jmeter spread the initial creation of threads over. Make sure \'${__P(warmup, 0)}\' is set as the value for the \'Ramp-Up Period\' setting in the \'Thread Group\' section of your Jmeter test.',
  );

  $form['start'] = array(
    '#type' => 'submit',
    '#value' => 'Review',
  );

  return $form;
}

function jmeter_add_job_form_start_validate($form, $form_state) {
  if(empty($form_state['values']['description'])) {
    form_set_error('description', 'You must enter a description');
  }
}
function jmeter_add_job_form_details_validate($form, $form_state) {
  if(empty($form_state['values']['threads'])) {
    form_set_error('threads', 'You must enter a positive integer in the threads field.');
  }
  else {
    if(!preg_match('/^[0-9]*$/', $form_state['values']['threads'])) {
      form_set_error('threads', 'Threads value must only contain numbers');
    }
    else {
      if($form_state['values']['threads'] > 100) {
        drupal_set_message('WARNING: Running a jmeter test with more than 100 threads is not recommended.', 'warning');
      }
    }
  }
}

function jmeter_add_job_form_submit($form, &$form_state) {
  error_log('submit handler called');
  if($form_state['stage'] == 'start') {
    $job = array(
      'id' => 0,
      'job_description' => $form_state['values']['description'],
      'java_parameters' => '',
      'jmeter_parameters' => '',
      'threads' => 0,
      'duration' => 0,
      'warmup' => 0,
    );

    $form_state['storage']['job'] = $job;

    if(drupal_write_record('jmeter_jobs', $job)) {
      // Create directories for this job.
      $upload_dir_created = file_prepare_directory(_jmeter_build_directory_path('uploads', $job['id']), FILE_CREATE_DIRECTORY);
      $results_dir_created = file_prepare_directory(_jmeter_build_directory_path('results', $job['id']), FILE_CREATE_DIRECTORY);

      if(!$upload_dir_created) {
        drupal_set_message('Could not create directory: ' . _jmeter_build_directory_path('uploads', $job['id']) . ', please check the ' . l('configuration page', 'admin/config/development/jmeter/configuration') . ' and try again', 'error');
      }
      else {
        if(variable_get('jmeter_debug_mode', '')) drupal_set_message('Created directory: ' . _jmeter_build_directory_path('uploads', $job['id']));
        $form_state['storage']['upload_dir'] = _jmeter_build_directory_path('uploads', $job['id']);
      }

      if(!$results_dir_created) {
        drupal_set_message('Could not create directory: ' . _jmeter_build_directory_path('results', $job['id']) . ', please check the ' . l('configuration page', 'admin/config/development/jmeter/configuration') . ' and try again', 'error');
      }
      else {
        if(variable_get('jmeter_debug_mode')) drupal_set_message('Created directory: ' . _jmeter_build_directory_path('results', $job['id']));
        $form_state['storage']['results_dir'] = _jmeter_build_directory_path('results', $job['id']);
      }

    }
    else {
      drupal_set_message('There was a problem adding your job to the database, please try again.', 'error');
    }

    $form_state['stage'] = 'details';
  }

  if($form_state['values']['stage'] == 'details') {
    error_log('we are in the submit for details');
  }

  $form_state['rebuild'] = TRUE;
}

function _jmeter_build_directory_path($type, $id) {
  return variable_get('jmeter_' . $type . '_directory', 'private://') . 'jmeter_' . $type . '/job_' . $id;
}

function jmeter_test($form, &$form_state) {
  error_log('function called');
}