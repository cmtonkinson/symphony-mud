-- $Id: 2-adding-creature-stats.sql 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
--
-- This file is part of the Symphony project <http://symphony.thetonk.com>
-- Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published
-- by the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

ALTER TABLE `avatars` ADD `exp` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `level`;
ALTER TABLE `avatars` ADD `tnl` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `exp`;
ALTER TABLE `avatars` ADD `hp` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `tnl`;
ALTER TABLE `avatars` ADD `maxHp` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `hp`;
ALTER TABLE `avatars` ADD `mana` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxHp`;
ALTER TABLE `avatars` ADD `maxMana` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `mana`;
ALTER TABLE `avatars` ADD `movement` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxMana`;
ALTER TABLE `avatars` ADD `maxMovement` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `movement`;
ALTER TABLE `avatars` ADD `strength` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxMovement`;
ALTER TABLE `avatars` ADD `maxStrength` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `strength`;
ALTER TABLE `avatars` ADD `dexterity` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxStrength`;
ALTER TABLE `avatars` ADD `maxDexterity` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `dexterity`;
ALTER TABLE `avatars` ADD `constitution` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxDexterity`;
ALTER TABLE `avatars` ADD `maxConstitution` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `constitution`;
ALTER TABLE `avatars` ADD `intelligence` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxConstitution`;
ALTER TABLE `avatars` ADD `maxIntelligence` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `intelligence`;
ALTER TABLE `avatars` ADD `wisdom` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxIntelligence`;
ALTER TABLE `avatars` ADD `maxWisdom` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `wisdom`;
ALTER TABLE `avatars` ADD `charisma` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxWisdom`;
ALTER TABLE `avatars` ADD `maxCharisma` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `charisma`;
ALTER TABLE `avatars` ADD `hitroll` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxCharisma`;
ALTER TABLE `avatars` ADD `damroll` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `hitroll`;
ALTER TABLE `avatars` ADD `saves` INT(10) NOT NULL DEFAULT 0 AFTER `damroll`;
ALTER TABLE `avatars` ADD `ac` INT(10) NOT NULL DEFAULT 0 AFTER `saves`;
ALTER TABLE `avatars` ADD `bash` INT(10) NOT NULL DEFAULT 0 AFTER `ac`;
ALTER TABLE `avatars` ADD `slash` INT(10) NOT NULL DEFAULT 0 AFTER `bash`;
ALTER TABLE `avatars` ADD `pierce` INT(10) NOT NULL DEFAULT 0 AFTER `slash`;
ALTER TABLE `avatars` ADD `exotic` INT(10) NOT NULL DEFAULT 0 AFTER `pierce`;

ALTER TABLE `mobs` ADD `exp` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `level`;
ALTER TABLE `mobs` ADD `tnl` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `exp`;
ALTER TABLE `mobs` ADD `hp` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `tnl`;
ALTER TABLE `mobs` ADD `maxHp` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `hp`;
ALTER TABLE `mobs` ADD `mana` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxHp`;
ALTER TABLE `mobs` ADD `maxMana` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `mana`;
ALTER TABLE `mobs` ADD `movement` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxMana`;
ALTER TABLE `mobs` ADD `maxMovement` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `movement`;
ALTER TABLE `mobs` ADD `strength` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxMovement`;
ALTER TABLE `mobs` ADD `maxStrength` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `strength`;
ALTER TABLE `mobs` ADD `dexterity` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxStrength`;
ALTER TABLE `mobs` ADD `maxDexterity` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `dexterity`;
ALTER TABLE `mobs` ADD `constitution` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxDexterity`;
ALTER TABLE `mobs` ADD `maxConstitution` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `constitution`;
ALTER TABLE `mobs` ADD `intelligence` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxConstitution`;
ALTER TABLE `mobs` ADD `maxIntelligence` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `intelligence`;
ALTER TABLE `mobs` ADD `wisdom` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxIntelligence`;
ALTER TABLE `mobs` ADD `maxWisdom` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `wisdom`;
ALTER TABLE `mobs` ADD `charisma` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxWisdom`;
ALTER TABLE `mobs` ADD `maxCharisma` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `charisma`;
ALTER TABLE `mobs` ADD `hitroll` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `maxCharisma`;
ALTER TABLE `mobs` ADD `damroll` INT(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `hitroll`;
ALTER TABLE `mobs` ADD `saves` INT(10) NOT NULL DEFAULT 0 AFTER `damroll`;
ALTER TABLE `mobs` ADD `ac` INT(10) NOT NULL DEFAULT 0 AFTER `saves`;
ALTER TABLE `mobs` ADD `bash` INT(10) NOT NULL DEFAULT 0 AFTER `ac`;
ALTER TABLE `mobs` ADD `slash` INT(10) NOT NULL DEFAULT 0 AFTER `bash`;
ALTER TABLE `mobs` ADD `pierce` INT(10) NOT NULL DEFAULT 0 AFTER `slash`;
ALTER TABLE `mobs` ADD `exotic` INT(10) NOT NULL DEFAULT 0 AFTER `pierce`;

