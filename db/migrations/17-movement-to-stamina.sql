ALTER TABLE `avatars` DROP `maxMovement`;
ALTER TABLE `avatars` CHANGE `movement` `stamina` INT(10) UNSIGNED NOT NULL DEFAULT '0';

ALTER TABLE `mobs` DROP `maxMovement`;
ALTER TABLE `mobs` CHANGE `movement` `stamina` INT(10) UNSIGNED NOT NULL DEFAULT '0';
