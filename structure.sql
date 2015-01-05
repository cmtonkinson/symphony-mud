-- $Id: structure.sql 381 2010-06-02 00:18:37Z cmtonkinson@gmail.com $
--
-- This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
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

-- phpMyAdmin SQL Dump
-- version 3.2.3
-- http://www.phpmyadmin.net
--
-- Host: drop.thetonk.com
-- Generation Time: Jun 01, 2010 at 10:55 AM
-- Server version: 5.1.46
-- PHP Version: 5.2.13

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `symphony`
--

-- --------------------------------------------------------

--
-- Table structure for table `areas`
--

DROP TABLE IF EXISTS `areas`;
CREATE TABLE IF NOT EXISTS `areas` (
  `areaID` int(11) NOT NULL AUTO_INCREMENT,
  `low` int(11) NOT NULL COMMENT 'vnum',
  `high` int(11) NOT NULL COMMENT 'vnum',
  `name` varchar(255) NOT NULL DEFAULT 'Undefined',
  `terrain` varchar(255) NOT NULL DEFAULT 'city',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`areaID`),
  UNIQUE KEY `low` (`low`),
  UNIQUE KEY `high` (`high`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `avatars`
--

DROP TABLE IF EXISTS `avatars`;
CREATE TABLE IF NOT EXISTS `avatars` (
  `avatarID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `active` tinyint(1) NOT NULL DEFAULT '1',
  `delete` tinyint(1) NOT NULL DEFAULT '0',
  `shortname` varchar(255) NOT NULL,
  `description` text NOT NULL,
  `password` varchar(255) NOT NULL,
  `gender` tinyint(4) NOT NULL DEFAULT '2',
  `race` int(10) unsigned NOT NULL DEFAULT '0',
  `cClass` int(10) unsigned NOT NULL DEFAULT '0',
  `totem` int(10) unsigned NOT NULL DEFAULT '0',
  `level` int(11) unsigned NOT NULL DEFAULT '1',
  `exp` int(10) unsigned NOT NULL DEFAULT '1',
  `tnl` int(10) unsigned NOT NULL DEFAULT '0',
  `hp` int(10) unsigned NOT NULL DEFAULT '0',
  `maxHp` int(10) unsigned NOT NULL DEFAULT '0',
  `mana` int(10) unsigned NOT NULL DEFAULT '0',
  `maxMana` int(10) unsigned NOT NULL DEFAULT '0',
  `movement` int(10) unsigned NOT NULL DEFAULT '0',
  `maxMovement` int(10) unsigned NOT NULL DEFAULT '0',
  `strength` int(10) unsigned NOT NULL DEFAULT '0',
  `maxStrength` int(10) unsigned NOT NULL DEFAULT '0',
  `dexterity` int(10) unsigned NOT NULL DEFAULT '0',
  `maxDexterity` int(10) unsigned NOT NULL DEFAULT '0',
  `constitution` int(10) unsigned NOT NULL DEFAULT '0',
  `maxConstitution` int(10) unsigned NOT NULL DEFAULT '0',
  `intelligence` int(10) unsigned NOT NULL DEFAULT '0',
  `maxIntelligence` int(10) unsigned NOT NULL DEFAULT '0',
  `wisdom` int(10) unsigned NOT NULL DEFAULT '0',
  `maxWisdom` int(10) unsigned NOT NULL DEFAULT '0',
  `charisma` int(10) unsigned NOT NULL DEFAULT '0',
  `maxCharisma` int(10) unsigned NOT NULL DEFAULT '0',
  `hitroll` int(10) unsigned NOT NULL DEFAULT '0',
  `damroll` int(10) unsigned NOT NULL DEFAULT '0',
  `saves` int(10) NOT NULL DEFAULT '0',
  `ac` int(10) NOT NULL DEFAULT '0',
  `bash` int(10) NOT NULL DEFAULT '0',
  `slash` int(10) NOT NULL DEFAULT '0',
  `pierce` int(10) NOT NULL DEFAULT '0',
  `exotic` int(10) NOT NULL DEFAULT '0',
  `practices` int(10) unsigned NOT NULL DEFAULT '0',
  `trains` int(10) unsigned NOT NULL DEFAULT '0',
  `gains` int(10) unsigned NOT NULL DEFAULT '0',
  `age` int(10) unsigned NOT NULL DEFAULT '17',
  `points` int(10) unsigned NOT NULL DEFAULT '0',
  `gold` int(10) unsigned NOT NULL DEFAULT '0',
  `silver` int(10) unsigned NOT NULL DEFAULT '0',
  `bank_gold` int(10) unsigned NOT NULL DEFAULT '0',
  `bank_silver` int(10) unsigned NOT NULL DEFAULT '0',
  `adminFlags` bigint(32) NOT NULL DEFAULT '0',
  `gechoColor` char(1) NOT NULL DEFAULT 'x',
  `channelFlags` bigint(32) NOT NULL DEFAULT '0',
  `whoFlags` bigint(32) NOT NULL DEFAULT '0',
  `room` int(11) unsigned NOT NULL COMMENT 'vnum',
  `title` varchar(255) NOT NULL,
  `poofin` varchar(255) NOT NULL,
  `poofout` varchar(255) NOT NULL,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`avatarID`),
  UNIQUE KEY `avatarName` (`shortname`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `disabled_commands`
--

DROP TABLE IF EXISTS `disabled_commands`;
CREATE TABLE IF NOT EXISTS `disabled_commands` (
  `table` enum('x','A','M','O','P','R','T') NOT NULL,
  `name` varchar(255) NOT NULL,
  UNIQUE KEY `tableName` (`table`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `exits`
--

DROP TABLE IF EXISTS `exits`;
CREATE TABLE IF NOT EXISTS `exits` (
  `exitID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `vnum` int(11) unsigned NOT NULL,
  `target` int(11) unsigned NOT NULL COMMENT 'vnum',
  `direction` tinyint(3) unsigned NOT NULL,
  `flags` bigint(32) NOT NULL DEFAULT '0',
  `key` int(11) NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`exitID`),
  KEY `roomID` (`vnum`),
  KEY `targetID` (`target`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `load_rules`
--

DROP TABLE IF EXISTS `load_rules`;
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

-- --------------------------------------------------------

--
-- Table structure for table `mobs`
--

DROP TABLE IF EXISTS `mobs`;
CREATE TABLE IF NOT EXISTS `mobs` (
  `mobID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `areaID` int(10) unsigned NOT NULL,
  `vnum` int(10) unsigned NOT NULL,
  `shortname` varchar(255) NOT NULL,
  `longname` varchar(255) NOT NULL,
  `keywords` varchar(255) NOT NULL,
  `description` text NOT NULL,
  `level` int(11) unsigned NOT NULL,
  `exp` int(10) unsigned NOT NULL DEFAULT '0',
  `tnl` int(10) unsigned NOT NULL DEFAULT '0',
  `hp` int(10) unsigned NOT NULL DEFAULT '0',
  `maxHp` int(10) unsigned NOT NULL DEFAULT '0',
  `mana` int(10) unsigned NOT NULL DEFAULT '0',
  `maxMana` int(10) unsigned NOT NULL DEFAULT '0',
  `movement` int(10) unsigned NOT NULL DEFAULT '0',
  `maxMovement` int(10) unsigned NOT NULL DEFAULT '0',
  `strength` int(10) unsigned NOT NULL DEFAULT '0',
  `maxStrength` int(10) unsigned NOT NULL DEFAULT '0',
  `dexterity` int(10) unsigned NOT NULL DEFAULT '0',
  `maxDexterity` int(10) unsigned NOT NULL DEFAULT '0',
  `constitution` int(10) unsigned NOT NULL DEFAULT '0',
  `maxConstitution` int(10) unsigned NOT NULL DEFAULT '0',
  `intelligence` int(10) unsigned NOT NULL DEFAULT '0',
  `maxIntelligence` int(10) unsigned NOT NULL DEFAULT '0',
  `wisdom` int(10) unsigned NOT NULL DEFAULT '0',
  `maxWisdom` int(10) unsigned NOT NULL DEFAULT '0',
  `charisma` int(10) unsigned NOT NULL DEFAULT '0',
  `maxCharisma` int(10) unsigned NOT NULL DEFAULT '0',
  `hitroll` int(10) unsigned NOT NULL DEFAULT '0',
  `damroll` int(10) unsigned NOT NULL DEFAULT '0',
  `saves` int(10) NOT NULL DEFAULT '0',
  `ac` int(10) NOT NULL DEFAULT '0',
  `bash` int(10) NOT NULL DEFAULT '0',
  `slash` int(10) NOT NULL DEFAULT '0',
  `pierce` int(10) NOT NULL DEFAULT '0',
  `exotic` int(10) NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`mobID`),
  UNIQUE KEY `vnum` (`vnum`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `notes`
--

DROP TABLE IF EXISTS `notes`;
CREATE TABLE IF NOT EXISTS `notes` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `board` int(10) unsigned NOT NULL,
  `author` int(10) unsigned NOT NULL COMMENT 'avatarID',
  `subject` varchar(255) NOT NULL,
  `body` text NOT NULL,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `objects`
--

DROP TABLE IF EXISTS `objects`;
CREATE TABLE IF NOT EXISTS `objects` (
  `objectID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `areaID` int(11) unsigned NOT NULL,
  `vnum` int(11) unsigned NOT NULL,
  `type` int(11) unsigned NOT NULL,
  `flags` bigint(20) unsigned NOT NULL,
  `composition` varchar(255) NOT NULL,
  `level` int(11) unsigned NOT NULL,
  `value` int(11) unsigned NOT NULL,
  `wearable` int(10) unsigned NOT NULL DEFAULT '0',
  `modifiers` varchar(255) NOT NULL,
  `keywords` varchar(255) NOT NULL,
  `shortname` varchar(255) NOT NULL,
  `longname` varchar(255) NOT NULL,
  `description` text NOT NULL,
  `furniture_capacity` int(10) unsigned NOT NULL DEFAULT '0',
  `furniture_lay_on` int(10) unsigned NOT NULL DEFAULT '0',
  `furniture_sit_at` int(10) unsigned NOT NULL DEFAULT '0',
  `furniture_sit_on` int(10) unsigned NOT NULL DEFAULT '0',
  `furniture_stand_on` int(10) unsigned NOT NULL DEFAULT '0',
  `weapon_type` int(10) unsigned NOT NULL DEFAULT '0',
  `weapon_verb` int(10) unsigned NOT NULL DEFAULT '0',
  `weapon_damage_number` int(10) unsigned NOT NULL DEFAULT '0',
  `weapon_damage_faces` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`objectID`),
  UNIQUE KEY `vnum` (`vnum`),
  KEY `areaID` (`areaID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `object_instances`
--

DROP TABLE IF EXISTS `object_instances`;
CREATE TABLE IF NOT EXISTS `object_instances` (
  `hash` varchar(255) NOT NULL,
  `in` varchar(255) NOT NULL COMMENT 'hash of container holding this object (if any)',
  `placement` enum('AVATAR','MOB','ROOM','CONTAINER') NOT NULL,
  `id` int(11) unsigned NOT NULL,
  `location` int(11) unsigned NOT NULL COMMENT '0 means inventory; non-zero means equipped',
  `order` int(11) unsigned NOT NULL COMMENT 'relative order, for inventory and room',
  `objectID` int(11) unsigned NOT NULL,
  `vnum` int(11) unsigned NOT NULL,
  `type` int(11) unsigned NOT NULL,
  `flags` bigint(20) unsigned NOT NULL,
  `composition` varchar(255) NOT NULL,
  `level` int(11) unsigned NOT NULL,
  `value` int(11) unsigned NOT NULL,
  `wearable` int(10) unsigned NOT NULL DEFAULT '0',
  `modifiers` varchar(255) NOT NULL,
  `keywords` varchar(255) NOT NULL,
  `shortname` varchar(255) NOT NULL,
  `longname` varchar(255) NOT NULL,
  `description` text NOT NULL,
  `furniture_capacity` int(10) unsigned NOT NULL DEFAULT '0',
  `furniture_lay_on` int(10) unsigned NOT NULL DEFAULT '0',
  `furniture_sit_at` int(10) unsigned NOT NULL DEFAULT '0',
  `furniture_sit_on` int(10) unsigned NOT NULL DEFAULT '0',
  `furniture_stand_on` int(10) unsigned NOT NULL DEFAULT '0',
  `weapon_type` int(10) unsigned NOT NULL DEFAULT '0',
  `weapon_verb` int(10) unsigned NOT NULL DEFAULT '0',
  `weapon_damage_number` int(10) unsigned NOT NULL DEFAULT '0',
  `weapon_damage_faces` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  KEY `id` (`id`),
  KEY `location` (`location`),
  KEY `order` (`order`),
  KEY `placement` (`placement`),
  KEY `hash` (`hash`),
  KEY `in` (`in`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='first row must has hash/in values of "none"/"none"';

-- --------------------------------------------------------

--
-- Table structure for table `permissions`
--

DROP TABLE IF EXISTS `permissions`;
CREATE TABLE IF NOT EXISTS `permissions` (
  `areaID` int(11) unsigned NOT NULL,
  `avatarID` int(11) unsigned NOT NULL,
  UNIQUE KEY `areaID` (`areaID`,`avatarID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `player_log`
--

DROP TABLE IF EXISTS `player_log`;
CREATE TABLE IF NOT EXISTS `player_log` (
  `logID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `level` int(11) unsigned NOT NULL,
  `type` int(11) NOT NULL,
  `text` text NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`logID`),
  KEY `level` (`level`),
  KEY `type` (`type`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `rooms`
--

DROP TABLE IF EXISTS `rooms`;
CREATE TABLE IF NOT EXISTS `rooms` (
  `roomID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `areaID` int(11) unsigned NOT NULL,
  `vnum` int(11) unsigned NOT NULL,
  `name` varchar(255) NOT NULL DEFAULT 'Undefined',
  `description` text NOT NULL,
  `smell` varchar(255) NOT NULL,
  `sound` varchar(255) NOT NULL,
  `terrain` varchar(255) NOT NULL,
  `flags` bigint(32) NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`roomID`),
  UNIQUE KEY `vnum` (`vnum`),
  KEY `areaID` (`areaID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `socials`
--

DROP TABLE IF EXISTS `socials`;
CREATE TABLE IF NOT EXISTS `socials` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `creator` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `targetNone` tinyint(1) NOT NULL DEFAULT '0',
  `targetSelf` tinyint(1) NOT NULL DEFAULT '0',
  `targetVictim` tinyint(1) NOT NULL DEFAULT '0',
  `noneActor` varchar(255) NOT NULL DEFAULT 'You do something.',
  `noneRoom` varchar(255) NOT NULL DEFAULT '$c does something.',
  `selfActor` varchar(255) NOT NULL DEFAULT 'You do something to yourself.',
  `selfRoom` varchar(255) NOT NULL DEFAULT '$c does something to $f.',
  `victimActor` varchar(255) NOT NULL DEFAULT 'You do something to $C.',
  `victimVictim` varchar(255) NOT NULL DEFAULT '$c does something to you.',
  `victimRoom` varchar(255) NOT NULL DEFAULT '$c does something to $C.',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `avatar` (`creator`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `world_log`
--

DROP TABLE IF EXISTS `world_log`;
CREATE TABLE IF NOT EXISTS `world_log` (
  `logID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `level` int(11) unsigned NOT NULL,
  `type` int(11) NOT NULL,
  `text` text NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`logID`),
  KEY `level` (`level`),
  KEY `type` (`type`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `object_instances`
--
ALTER TABLE `object_instances`
  ADD CONSTRAINT `object_instances_ibfk_1` FOREIGN KEY (`in`) REFERENCES `object_instances` (`hash`) ON DELETE CASCADE;
