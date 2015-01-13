-- $Id: 5-more-avatar-stats.sql 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

ALTER TABLE `avatars` ADD `practices` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `exotic`,
ADD `trains` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `practices`,
ADD `gains` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `trains`,
ADD `age` INT UNSIGNED NOT NULL DEFAULT '17' AFTER `gains`,
ADD `points` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `age`,
ADD `gold`  INT UNSIGNED NOT NULL DEFAULT '0' AFTER `points`,
ADD `silver` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `gold`,
ADD `bank_gold` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `silver`,
ADD `bank_silver` INT UNSIGNED NOT NULL DEFAULT '0' AFTER `bank_gold`
;
