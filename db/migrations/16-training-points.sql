-- What the flying fuck were all these for?!
ALTER TABLE `avatars` DROP `practices`;
ALTER TABLE `avatars` DROP `trains`;
ALTER TABLE `avatars` DROP `gains`;
ALTER TABLE `avatars` DROP `points`;

ALTER TABLE `avatars` ADD `trains` INT(10) UNSIGNED NOT NULL DEFAULT '0';
