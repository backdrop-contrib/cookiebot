/**
 * @file
 * Cookiebot functionality.
 */

(function ($, Drupal) {
  'use strict';

  var $window = $(window);
  var renewConsentTriggerEventName = 'cookiebotConsentRenew';

  /**
   * Capitalize the first character of a given string.
   *
   * @param {string} string
   *   The string to capitalize the first character of.
   *
   * @return {string}
   *   The string with the first letter capitalized.
   */
  var capitalizeFirstCharacter = function (string) {
    return string[0].toUpperCase() + string.substr(1);
  };

  /**
   * Listens to event of a user accepting the use of cookies.
   *
   * This is also called on every page load when cookies are already accepted.
   */
  $window.on('CookiebotOnAccept', function () {
    Drupal.cookiebot.updateCookies();
  });

  /**
   * Listens to the event of a user declining the use of cookies.
   *
   * This is also called on every page load when cookies are already declined.
   */
  $window.on('CookiebotOnDecline', function () {
    Drupal.cookiebot.updateCookies();
  });

  /**
   * Listens to event of a user wanting to change their cookies consent.
   */
  $window.on(renewConsentTriggerEventName, function () {
    if (typeof Cookiebot === 'undefined') {
      return;
    }

    Cookiebot.renew();
  });

  /**
   * Attach Cookiebot renew click event listener.
   *
   * @type {Drupal~behavior}
   */
  Drupal.behaviors.cookiebot = {
    attach: function attach(context, settings) {
      Drupal.cookiebot.updateCookies();

      $('.cookiebot-renew', context).once().on('click', function (event) {
        event.preventDefault();

        $window.trigger(renewConsentTriggerEventName);
      });

      if (settings.cookiebot.message_placeholder_cookieconsent_optout_marketing_show && settings.cookiebot.message_placeholder_cookieconsent_optout_marketing.length > 0) {
        var message_placeholder_cookieconsent_optout_marketing = settings.cookiebot.message_placeholder_cookieconsent_optout_marketing.replace('!cookiebot_renew', 'javascript:Cookiebot.renew()');
        $('[data-cookieconsent="marketing"][data-src]').each(function () {
          var cookiebot_from_src_url = '';
          if ($(this).attr('data-src').length) {
            cookiebot_from_src_url = $(this).attr('data-src');
          }
          $(this).once('cb-message-placeholder-cookieconsent-optout-marketing').after(message_placeholder_cookieconsent_optout_marketing.replace(new RegExp('!cookiebot_from_src_url', 'g'), cookiebot_from_src_url));
        });
      }
    }
  };

  /**
   * Updates cookies for Cookiebot.
   *
   * We set our own cookies to be able to provide integration with other Drupal
   * modules, without relying on the cookies of Cookiebot, since those are not
   * part of the public API.
   */
  Drupal.cookiebot = {
    updateCookies: function updateCookies() {
      var cookieNames = [
        'necessary',
        'preferences',
        'statistics',
        'marketing'
      ];

      if (typeof Cookiebot === 'undefined' || Cookiebot.consent === void (0)) {
        return;
      }

      $.each(cookieNames, function (index, cookieName) {
        if (Cookiebot.consent[cookieName] === true && $.cookie('cookiebot-consent--' + cookieName) !== '1') {
          $.cookie('cookiebot-consent--' + cookieName, '1', {
            path: '/'
          });
          $window.trigger('cookiebotConsentAccept' + capitalizeFirstCharacter(cookieName));
          return;
        }

        if (Cookiebot.consent[cookieName] === false && $.cookie('cookiebot-consent--' + cookieName) !== '0') {
          $.cookie('cookiebot-consent--' + cookieName, '0', {
            path: '/'
          });
          $window.trigger('cookiebotConsentDecline' + capitalizeFirstCharacter(cookieName));
        }
      });
    }
  };

})(jQuery, Drupal, window);
