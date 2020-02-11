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
function cookiebot_settings_form($form, &$form_state) {
  $cbid = variable_get('cookiebot_cbid', '');

  if (empty($cbid)) {
    drupal_set_message(t('Cookiebot functionality is disabled until you enter a valid CBID.'), 'warning');
  }

  $form['cookiebot_cbid'] = array(
    '#type' => 'textfield',
    '#title' => t('Your cookiebot Domain Group ID (CBID)'),
    '#description' => t("This ID looks like 00000000-0000-0000-0000-000000000000. You can find it in the <a href='@url'>Cookiebot Manager</a> on the 'Your scripts' tab.", array('@url' => 'https://www.cookiebot.com/en/manage')),
    '#default_value' => $cbid,
  );

  $form['cookiebot_block_cookies'] = [
    '#type' => 'checkbox',
    '#title' => t('Automatically block all cookies'),
    '#description' => t('Defines if Cookiebot should <a href="@automatic_url">automatically block all cookies</a> until a user has consented. If not set, cookie-setting scripts should manually be marked up as described in the <a href="@manual_url">manual implementation guide</a>.', [
      '@automatic_url' => 'https://www.cookiebot.com/en/automatic-cookie-control/',
      '@manual_url' => 'https://www.cookiebot.com/en/manual-implementation/',
    ]),
    '#default_value' => variable_get('cookiebot_block_cookies', TRUE),
  ];

  $form['cookiebot_iab_enabled'] = [
    '#type' => 'checkbox',
    '#title' => t('Enabling IAB framework'),
    '#description' => t('IAB (Interactive Advertising Bureau) model puts scripts control in the hands of advertisers and vendors by only signaling consent to vendors. More information about <a href="https://support.cookiebot.com/hc/en-us/articles/360007652694-Cookiebot-and-the-IAB-Consent-Framework">Cookiebot and the IAB Consent Framework</a>.'),
    '#default_value' => variable_get('cookiebot_iab_enabled'),
  ];

  $show_declaration = variable_get('cookiebot_show_declaration', FALSE);

  $form['cookiebot_declaration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cookie declaration'),
    '#collapsible' => TRUE,
  );

  if (!$show_declaration) {
    $form['cookiebot_declaration']['#attributes']['class'][] = 'collapsed';
  }

  $description = t('Automatically show the full Cookiebot cookie declaration on the given page.');
  $description .= '<br />';
  $description .= t("Note that custom templates and modules like Panels and Display Suite can prevent the declaration from showing up.
  You can always place our block or manually place Cookiebot's declaration script found in their manager if your input filters allow it.");

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

  $form['visibility'] = [
    '#type' => 'fieldset',
    '#title' => t('Cookiebot visibility'),
  ];

  $form['visibility']['cookiebot_exclude_paths'] = [
    '#type' => 'textarea',
    '#title' => t('Exclude paths'),
    '#default_value' => variable_get('cookiebot_exclude_paths', ''),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", [
      '%blog' => '/blog',
      '%blog-wildcard' => '/blog/*',
      '%front' => '<front>',
    ]),
  ];

  $form['visibility']['cookiebot_exclude_admin_theme'] = [
    '#type' => 'checkbox',
    '#title' => t('Exclude admin pages'),
    '#default_value' => variable_get('cookiebot_exclude_admin_theme', FALSE),
  ];

  $form['visibility']['cookiebot_exclude_uid_1'] = [
    '#type' => 'checkbox',
    '#title' => t('Don’t show the Cookiebot for UID 1.'),
    '#default_value' => variable_get('cookiebot_exclude_uid_1', FALSE)
  ];

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
function cookiebot_settings_form_validate($form, &$form_state) {
  $cbid_trimmed = trim($form_state['values']['cookiebot_cbid']);
  $form_state['values']['cookiebot_cbid'] = $cbid_trimmed;

  if (!empty($cbid_trimmed) && !preg_match('/^[0-9a-z]{8}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{12}$/', $cbid_trimmed)) {
    form_set_error('cookiebot_cbid', t('The entered Domain Group ID is not formatted correctly.'));
  }

  if (!empty($form_state['values']['cookiebot_show_declaration_node_id']) && !node_load($form_state['values']['cookiebot_show_declaration_node_id'])) {
    form_set_error('cookiebot_show_declaration_node_id', t('The entered node ID is invalid.'));
  }

  // Clear caches like the system_performance_settings() form does.
  drupal_clear_css_cache();
  drupal_clear_js_cache();
  cache_clear_all('*', 'cache_page', TRUE);
}
