ALTER TABLE `objects`
 ADD  `weapon_type` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `description`,
 ADD  `weapon_verb` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `weapon_type`,
 ADD  `weapon_damage_number` INT UNSIGNED NOT NULL DEFAULT '0' AFTER  `weapon_verb`,
 ADD  `weapon_damage_faces` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `weapon_damage_number`
;

ALTER TABLE `object_instances`
 ADD  `weapon_type` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `description`,
 ADD  `weapon_verb` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `weapon_type`,
 ADD  `weapon_damage_number` INT UNSIGNED NOT NULL DEFAULT '0' AFTER  `weapon_verb`,
 ADD  `weapon_damage_faces` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `weapon_damage_number`
;
