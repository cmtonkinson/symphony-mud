ALTER TABLE `objects`
 ADD  `furniture_capacity` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `description` ,
 ADD  `furniture_lay_on` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `furniture_capacity` ,
 ADD  `furniture_sit_at` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `furniture_lay_on` ,
 ADD  `furniture_sit_on` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `furniture_sit_at` ,
 ADD  `furniture_stand_on` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `furniture_sit_on`
;

ALTER TABLE `object_instances`
 ADD  `furniture_capacity` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `description` ,
 ADD  `furniture_lay_on` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `furniture_capacity` ,
 ADD  `furniture_sit_at` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `furniture_lay_on` ,
 ADD  `furniture_sit_on` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `furniture_sit_at` ,
 ADD  `furniture_stand_on` INT UNSIGNED NOT NULL DEFAULT  '0' AFTER  `furniture_sit_on`
;
