<?php

/**
 * @file
 *
 * Admin callbacks for Islandora Preingest Transform
 */

/**
 * Admin form.
 */
function islandora_transform_admin($form, &$form_state) {
  $xslt = variable_get('islandora_mods_transform_xslt', FALSE);
  $message = $xslt ? 'Change XSLT' : 'Add XSLT';
  if ($xslt) {
    $form['wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Current XSLT'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['wrapper']['current'] = array(
      '#type' => 'item',
      '#markup' => "<pre>" . htmlentities("$xslt"). "</pre>",
    );
  }
  $form['xslt'] = array(
    '#type' => 'file',
    '#title' => t($message),
    '#description' => t('Change xslt used for transformations.')
      //'#required' => TRUE, // Always returns an error, even though a file is successfully uploaded
  );
  $form['islandora_transform_submit'] = array(
    '#value' => t('Upload xslt'),
    '#type' => 'submit',
  );
  return $form;
}

function islandora_transform_admin_validate($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('xsl', 'xslt'));
  $file = file_save_upload('xslt', $validators);
  if ($file) {
    if ($file = file_move($file, 'public://')) {
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file', t('Failed to write the uploaded file to the site\'s file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

function islandora_transform_admin_submit($form, &$form_state) {
  $file = $form_state['storage']['file'];
  unset($form_state['storage']['file']);
  variable_set("islandora_mods_transform_xslt", file_get_contents($file->uri));
  drupal_set_message(t('The form has been submitted and the image has been saved, filename: @filename.', array('@filename' => $file->filename)));
}
