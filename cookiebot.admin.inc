<?php

/**
 * @file
 * Admin functionality for Cookiebot.
 */

/**
 * Form callback for the admin settings form.
 *
 * @param array $form
 *   The structure of the form.
 * @param array $form_state
 *   The current state of the form.
 *
 * @return array
 *   The configuration form.
 *
 * @see system_settings_form_submit()
 */
function cookiebot_settings_form(array $form, array &$form_state) {
  $form['cookiebot_cbid'] = array(
    '#type' => 'textfield',
    '#title' => t('Your cookiebot Domain Group ID (CBID)'),
    '#description' => t("This ID looks like 00000000-0000-0000-0000-000000000000. You can find it in the <a href='https://www.cookiebot.com/en/manage'>Cookiebot Manager</a> on the 'Your scripts' tab."),
    '#default_value' => variable_get('cookiebot_cbid', ''),
  );

  $show_declaration = variable_get('cookiebot_show_declaration', FALSE);

  $form['cookiebot_declaration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cookie declaration'),
    '#attributes' => array('class' => array('collapsible')),
  );

  if (!$show_declaration) {
    $form['cookiebot_declaration']['#attributes']['class'][] = 'collapsed';
  }

  $description = t('Automatically show the full Cookiebot cookie declaration on the given page.');
  $description .= '<br />';
  $description .= t("Note that custom templates and modules like Panels and Display Suite can prevent the declaration from showing up. You can always manually place Cookiebot's declaration script found in their manager if your input filters allow it.");
  $form['cookiebot_declaration']['cookiebot_show_declaration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the Cookiebot cookie declaration'),
    '#description' => $description,
    '#default_value' => $show_declaration,
  );

  $form['cookiebot_declaration']['cookiebot_show_declaration_node_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Node ID'),
    '#description' => t('Show the full cookie declaration declaration on the node page with the given ID.'),
    '#default_value' => variable_get('cookiebot_show_declaration_node_id', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="cookiebot_show_declaration"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  drupal_add_library('system', 'drupal.collapse');

  return system_settings_form($form);
}

/**
 * Form validate callback for the admin settings form.
 *
 * @param array $form
 *   The structure of the form.
 * @param array $form_state
 *   The current state of the form.
 */
function cookiebot_settings_form_validate(array $form, array &$form_state) {
  $cbid_trimmed = trim($form_state['values']['cookiebot_cbid']);
  $form_state['values']['cookiebot_cbid'] = $cbid_trimmed;

  if (!preg_match('/^[0-9a-z]{8}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{12}$/', $cbid_trimmed)) {
    form_set_error('cookiebot_cbid', t('The entered Domain Group ID is not formatted correctly.'));
  }

  if (!empty($form_state['values']['cookiebot_show_declaration_node_id']) && !node_load($form_state['values']['cookiebot_show_declaration_node_id'])) {
    form_set_error('cookiebot_show_declaration_node_id', t('The entered node ID is invalid.'));
  }
}
