<?php

function jmeter_landing_page() {
  return 'this is the landing page';
}

function jmeter_settings_page_form($form, &$form_state) {
  $form = array();

  $form['file_locations'] = array(
    '#type' => 'fieldset',
    '#title' => t('File Locations'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['file_locations']['jmeter_upload_directory'] = array(
    '#title' => t('Jmeter upload directory'),
    '#type' => 'textfield',
    '#description' => '/jmeter_uploads will be appended to the file path specified. Directory specified should not be web accessible.',
    '#default_value' => variable_get('jmeter_upload_directory', 'private://'),
  );

  $form['file_locations']['jmeter_results_directory'] = array(
    '#title' => t('Jmeter results directory'),
    '#type' => 'textfield',
    '#description' => '/jmeter_results will be appended to the file path specified. Directory specified should not be web accessible.',
    '#default_value' => variable_get('jmeter_results_directory', 'private://'),
  );

  return system_settings_form($form);
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
  $form = array();

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
  $form = array();

  $form['heading'] = array(
    '#markup' => '<h2>Setup Job: ' . $form_state['values']['description'] . '</h2>',
  );

  $form['instructions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instructions'),
    '#collapsable' => FALSE,
  );

  $form['instructions']['instruction_text'] = array(
    '#markup' => 'Please upload all necessary files for your job. This includes the jmx file as well as any assets required by the jmeter test.',
  );

  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Files'),
    '#collapsable' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['files']['upload'] = array(
    '#type' => 'file',
    '#title' => 'Upload files',
    '#description' => 'Upload a file.',
  );

  $form['start'] = array(
    '#type' => 'submit',
    '#value' => 'Start',
  );

  return $form;
}

function jmeter_add_job_form_start_validate($form, $form_state) {
  if(empty($form_state['values']['description'])) {
    form_set_error('description', 'You must enter a description');
  }
}
function jmeter_add_job_form_details_validate($form, $form_state) {
  return TRUE;
}

function jmeter_add_job_form_submit($form, &$form_state) {
  if($form_state['stage'] == 'start') {
    $job = array(
      'id' => 0,
      'description' => $form_state['values']['description'],
      'java_commands' => '',
      'jmeter_commands' => '',
      'threads' => '',
      'warmup' => '',
      'duration' => ''
    );

    if(drupal_write_record('jmeter_jobs', $job)) {
      
    }


    $dir = drupal_mkdir('private://test');

    if($dir) {
      error_log('directory created');
    }
    else {
      error_log('directory not created');
    }


    $form_state['stage'] = 'details';
  }
  $form_state['rebuild'] = TRUE;
}