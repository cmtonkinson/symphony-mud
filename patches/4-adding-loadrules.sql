-- $Id: 4-adding-loadrules.sql 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

CREATE TABLE IF NOT EXISTS `load_rules` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `vnum` int(10) unsigned NOT NULL,
  `type` enum('MOB','OBJECT') NOT NULL,
  `target` int(10) unsigned NOT NULL,
  `number` int(10) unsigned NOT NULL DEFAULT '1',
  `max` int(10) unsigned NOT NULL DEFAULT '1',
  `probability` int(10) unsigned NOT NULL DEFAULT '100',
  `preposition` enum('NONE','ON','IN','CARRY','WEAR') NOT NULL DEFAULT 'NONE',
  `indirect_object` int(11) NOT NULL,
  `indirect_object_index` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

