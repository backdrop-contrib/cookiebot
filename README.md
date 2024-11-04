Cookiebot
======================

This module offers Drupal integration for the third party Cookiebot service.
Cookiebot helps make your use of cookies and online tracking GDPR and ePR
compliant.

Requirements
------------

You will need a Cookiebot account with a configured domain.
You can register via one of this module's supporting organisations to support
development of this module.

[Sign up!](https://manage.cookiebot.com/goto/signup?rid=0N85W)
(Synetic's referral)

Then, grab your _Domain Group ID_ (CBID) from the 'Your scripts' tab.

This module will only run with jQuery 1.9 and higher.

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://docs.backdropcms.org/documentation/extend-with-modules.
- Visit /admin/config/system/cookiebot
 - Set your _Domain Group ID_.
 - You can optionally display the full cookie declaration on a specific node
   page or place our block via admin/structure/block (Panels supported).

### Cookiebot renew

To allow users to change cookiebot settings, you can add a menu link with
URL placeholder "<cookiebot-renew>" or a link anywhere with a class
`cookiebot-renew`.

The Cookiebot interface will show on click.

Issues
------

Bugs and feature requests should be reported in [the Issue Queue](https://github.com/backdrop-contrib/cookiebot/issues).

Current Maintainers
-------------------

- [Eli Lisseck](https://github.com/elisseck).
- [Anthony Nemirovsky](https://github.com/anemirovsky).

Credits
-------

- Backdrop development supported by [USENIX](https://www.usenix.org/).
- Backdrop development supported by [Giant Rabbit](https://giantrabbit.com).
- Originally written and maintained for Drupal by [Alex Milkovskyi](https://git.drupalcode.org/milkovsky), [Bram Driesen](https://git.drupalcode.org/BramDriesen), [Daniël Smidt](https://git.drupalcode.org/MrDamasajaga), [Julian Pustkuchen](https://git.drupalcode.org/Anybody), [Mark Conroy](https://git.drupalcode.org/MarkConroy), [Steven Buteneers](https://git.drupalcode.org/StevenButeneers).
- Ported to Backdrop CMS by [Christopher Yu]().

License
-------

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.


