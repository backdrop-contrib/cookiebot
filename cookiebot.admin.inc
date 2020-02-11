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

  $form['placeholders'] = [
    '#type' => 'fieldset',
    '#title' => t('Placeholders for blocked elements with src attribute (iframe, etc.)'),
    '#description' => t('Define placeholders for blocked ´src´ elements like iframe, img, audio, video, embed, picture, source. In automatic mode some sources like YouTube iFrames are blocked automatically. In manual mode you have to add the markup yourself. See Cookiebot support <a href="@url1" target="_blank">here</a> and <a href="@url2" target="_blank">here</a> for details.', ['@url1'=>'https://support.cookiebot.com/hc/en-us/articles/360003790854-Iframe-cookie-consent-with-YouTube-example', '@url2' => 'https://support.cookiebot.com/hc/en-us/articles/360003812053-Hide-and-show-content-based-on-the-visitor-s-consent']),
  ];
  $form['placeholders']['marketing'] = [
    '#type' => 'details',
    '#title' => t('Marketing') . ' ' . '([data-src][data-cookieconsent="marketing"])',
    '#description' => t('Blocked elements with [data-src][data-cookieconsent="marketing"] attributes. This is typically automatically added by Cookiebot in automatic mode.'),
  ];
  $form['placeholders']['marketing']['cookiebot_message_placeholder_cookieconsent_optout_marketing_show'] = [
    '#type' => 'checkbox',
    '#title' => t('Show placeholder message for blocked marketing elements'),
    '#description' => t('Select if you want to show a message for blocked elements like iframes, hidden by cookiebot until marketing consent is given.'),
    '#default_value' => variable_get('cookiebot_message_placeholder_cookieconsent_optout_marketing_show', FALSE),
  ];

  $cookiebot_message_placeholder_cookieconsent_optout_marketing = variable_get('cookiebot_message_placeholder_cookieconsent_optout_marketing');
  $cookiebot_message_placeholder_cookieconsent_optout_marketing_text_default = 'Please <a href="!cookiebot_renew" class="cookieconsent-optout-marketing__cookiebot-renew">accept marketing-cookies</a> to view this embedded content from <a href="!cookiebot_from_src_url" target="_blank" class="cookieconsent-optout-marketing__from-src-url">!cookiebot_from_src_url</a>';
  $form['placeholders']['marketing']['cookiebot_message_placeholder_cookieconsent_optout_marketing'] = [
    '#type' => 'text_format',
    '#title' => t('Placebolder message for blocked marketing elements'),
    '#default_value' => (!empty($cookiebot_message_placeholder_cookieconsent_optout_marketing) ? $cookiebot_message_placeholder_cookieconsent_optout_marketing['value'] : $cookiebot_message_placeholder_cookieconsent_optout_marketing_text_default),
    '#required' => FALSE,
    '#description' => t(
      "Add this placeholder below the blocked marketing element, if the user has not consented to marketing cookies.<br />Clear to use the default markup.<br />You may use these dynamical placeholders: <ul><li><em>!cookiebot_renew</em> = javascript:Cookiebot.renew()</li><li><em>!cookiebot_from_src_url</em> = iframe data-src attribute value</li></ul>"
    ),
    '#format' => (!empty($cookiebot_message_placeholder_cookieconsent_optout_marketing['format']) ? $cookiebot_message_placeholder_cookieconsent_optout_marketing['format'] : filter_default_format()),
    '#states' => [
      'visible' => [
        ':input[name="cookiebot_message_placeholder_cookieconsent_optout_marketing_show"]' => [
          'checked' => TRUE,
        ],
      ],
    ],
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
