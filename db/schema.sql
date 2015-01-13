-- MySQL dump 10.13  Distrib 5.5.40, for debian-linux-gnu (i686)
--
-- Host: localhost    Database: symphony
-- ------------------------------------------------------
-- Server version	5.5.40-0ubuntu0.14.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `abilities`
--

DROP TABLE IF EXISTS `abilities`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `abilities` (
  `avatar` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `mastery` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  KEY `avatar` (`avatar`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `areas`
--

DROP TABLE IF EXISTS `areas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `areas` (
  `areaID` int(11) NOT NULL AUTO_INCREMENT,
  `low` int(11) NOT NULL COMMENT 'vnum',
  `high` int(11) NOT NULL COMMENT 'vnum',
  `name` varchar(255) NOT NULL DEFAULT 'Undefined',
  `terrain` varchar(255) NOT NULL DEFAULT 'city',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`areaID`),
  UNIQUE KEY `low` (`low`),
  UNIQUE KEY `high` (`high`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `avatars`
--

DROP TABLE IF EXISTS `avatars`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `avatars` (
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
  `health` int(10) unsigned NOT NULL DEFAULT '0',
  `maxHealth` int(10) unsigned NOT NULL DEFAULT '0',
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
  `focus` int(10) unsigned NOT NULL DEFAULT '0',
  `maxFocus` int(10) unsigned NOT NULL DEFAULT '0',
  `charisma` int(10) unsigned NOT NULL DEFAULT '0',
  `maxCharisma` int(10) unsigned NOT NULL DEFAULT '0',
  `armor` int(10) unsigned NOT NULL DEFAULT '0',
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
  `creativity` int(10) unsigned NOT NULL DEFAULT '0',
  `maxCreativity` int(10) unsigned NOT NULL DEFAULT '0',
  `luck` int(10) unsigned NOT NULL DEFAULT '0',
  `maxLuck` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`avatarID`),
  UNIQUE KEY `avatarName` (`shortname`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `disabled_commands`
--

DROP TABLE IF EXISTS `disabled_commands`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `disabled_commands` (
  `table` enum('x','A','M','O','P','R','T') NOT NULL,
  `name` varchar(255) NOT NULL,
  UNIQUE KEY `tableName` (`table`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `exits`
--

DROP TABLE IF EXISTS `exits`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `exits` (
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
) ENGINE=MyISAM AUTO_INCREMENT=39 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `load_rules`
--

DROP TABLE IF EXISTS `load_rules`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `load_rules` (
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `mobs`
--

DROP TABLE IF EXISTS `mobs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mobs` (
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
  `health` int(10) unsigned NOT NULL DEFAULT '0',
  `maxHealth` int(10) unsigned NOT NULL DEFAULT '0',
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
  `focus` int(10) unsigned NOT NULL DEFAULT '0',
  `maxFocus` int(10) unsigned NOT NULL DEFAULT '0',
  `charisma` int(10) unsigned NOT NULL DEFAULT '0',
  `maxCharisma` int(10) unsigned NOT NULL DEFAULT '0',
  `armor` int(10) unsigned NOT NULL DEFAULT '0',
  `bash` int(10) NOT NULL DEFAULT '0',
  `slash` int(10) NOT NULL DEFAULT '0',
  `pierce` int(10) NOT NULL DEFAULT '0',
  `exotic` int(10) NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `creativity` int(10) unsigned NOT NULL DEFAULT '0',
  `maxCreativity` int(10) unsigned NOT NULL DEFAULT '0',
  `luck` int(10) unsigned NOT NULL DEFAULT '0',
  `maxLuck` int(10) unsigned NOT NULL DEFAULT '0',
  `gender` int(10) unsigned NOT NULL DEFAULT '1',
  `race` int(10) unsigned NOT NULL DEFAULT '1',
  `pClass` int(10) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`mobID`),
  UNIQUE KEY `vnum` (`vnum`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `notes`
--

DROP TABLE IF EXISTS `notes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `notes` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `board` int(10) unsigned NOT NULL,
  `author` int(10) unsigned NOT NULL COMMENT 'avatarID',
  `subject` varchar(255) NOT NULL,
  `body` text NOT NULL,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `object_instances`
--

DROP TABLE IF EXISTS `object_instances`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `object_instances` (
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
  KEY `in` (`in`),
  CONSTRAINT `object_instances_ibfk_1` FOREIGN KEY (`in`) REFERENCES `object_instances` (`hash`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='first row must has hash/in values of "none"/"none"';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `objects`
--

DROP TABLE IF EXISTS `objects`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `objects` (
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `permissions`
--

DROP TABLE IF EXISTS `permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permissions` (
  `areaID` int(11) unsigned NOT NULL,
  `avatarID` int(11) unsigned NOT NULL,
  UNIQUE KEY `areaID` (`areaID`,`avatarID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_log`
--

DROP TABLE IF EXISTS `player_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_log` (
  `logID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `level` int(11) unsigned NOT NULL,
  `type` int(11) NOT NULL,
  `text` text NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`logID`),
  KEY `level` (`level`),
  KEY `type` (`type`)
) ENGINE=MyISAM AUTO_INCREMENT=19 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rooms`
--

DROP TABLE IF EXISTS `rooms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rooms` (
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
) ENGINE=MyISAM AUTO_INCREMENT=19 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `socials`
--

DROP TABLE IF EXISTS `socials`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `socials` (
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
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `world_log`
--

DROP TABLE IF EXISTS `world_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `world_log` (
  `logID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `level` int(11) unsigned NOT NULL,
  `type` int(11) NOT NULL,
  `text` text NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`logID`),
  KEY `level` (`level`),
  KEY `type` (`type`)
) ENGINE=MyISAM AUTO_INCREMENT=68 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-01-13 15:12:57
