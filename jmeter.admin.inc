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

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => 'Job Description',
    '#description' => 'A descriptive title for this job.',
  );


  return $form;
}
