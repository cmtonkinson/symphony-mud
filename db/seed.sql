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
-- Dumping data for table `abilities`
--

LOCK TABLES `abilities` WRITE;
/*!40000 ALTER TABLE `abilities` DISABLE KEYS */;
INSERT INTO `abilities` VALUES (1,'second strike',25,'2015-01-14 19:50:04'),(1,'block',25,'2015-01-14 19:50:04'),(3,'spark',25,'2015-01-14 18:13:21'),(3,'flame',25,'2015-01-14 18:13:21'),(3,'frost',25,'2015-01-14 18:13:21');
/*!40000 ALTER TABLE `abilities` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `areas`
--

LOCK TABLES `areas` WRITE;
/*!40000 ALTER TABLE `areas` DISABLE KEYS */;
INSERT INTO `areas` VALUES (1,0,999,'The Tundra','snow','2015-01-12 17:29:44');
/*!40000 ALTER TABLE `areas` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `avatars`
--

LOCK TABLES `avatars` WRITE;
/*!40000 ALTER TABLE `avatars` DISABLE KEYS */;
INSERT INTO `avatars` VALUES (1,1,0,'Xantiloth','','*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19',1,3,4,0,125,0,0,6097,6097,472,472,100,17,22,16,21,17,22,13,18,13,18,15,20,100,0,0,0,0,17,0,0,0,0,0,'x',0,0,3,', the Absent Minded','','','2015-01-14 19:50:04',15,20,15,20,126),(2,1,0,'Luigi','','*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19',1,2,3,0,25,0,2969,268,268,184,184,100,15,20,19,24,13,18,17,22,15,20,15,20,100,0,0,0,0,17,0,0,0,0,0,'x',0,0,3,', adventurer.','','','2015-01-14 19:50:04',16,21,15,20,29),(3,1,0,'Joe','','*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19',1,2,2,0,5,0,132,114,114,100,115,112,13,18,17,22,12,17,18,23,17,22,17,22,100,0,0,0,0,17,0,0,0,0,0,'x',0,0,3,', adventurer.','','','2015-01-14 18:13:21',17,22,15,20,6);
/*!40000 ALTER TABLE `avatars` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `disabled_commands`
--

LOCK TABLES `disabled_commands` WRITE;
/*!40000 ALTER TABLE `disabled_commands` DISABLE KEYS */;
/*!40000 ALTER TABLE `disabled_commands` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `exits`
--

LOCK TABLES `exits` WRITE;
/*!40000 ALTER TABLE `exits` DISABLE KEYS */;
INSERT INTO `exits` VALUES (1,0,1,1,0,0,'2015-01-12 17:30:22'),(2,1,0,3,0,0,'2015-01-12 17:30:22'),(3,1,2,2,0,0,'2015-01-12 17:30:24'),(4,2,1,4,0,0,'2015-01-12 17:30:24'),(5,2,3,3,0,0,'2015-01-12 17:30:31'),(6,3,2,1,0,0,'2015-01-12 17:30:31'),(7,3,4,2,0,0,'2015-01-12 17:30:32'),(8,4,3,4,0,0,'2015-01-12 17:30:32'),(9,4,5,2,0,0,'2015-01-12 17:30:33'),(10,5,4,4,0,0,'2015-01-12 17:30:33'),(11,5,6,2,0,0,'2015-01-12 17:30:33'),(12,6,5,4,0,0,'2015-01-12 17:30:33'),(13,6,7,2,0,0,'2015-01-12 17:30:33'),(14,7,6,4,0,0,'2015-01-12 17:30:33'),(15,7,8,2,0,0,'2015-01-12 17:30:34'),(16,8,7,4,0,0,'2015-01-12 17:30:34'),(17,8,9,2,0,0,'2015-01-12 17:30:34'),(18,9,8,4,0,0,'2015-01-12 17:30:34'),(19,9,10,3,0,0,'2015-01-12 17:30:37'),(20,10,9,1,0,0,'2015-01-12 17:30:37'),(21,10,11,4,0,0,'2015-01-12 17:30:38'),(22,11,10,2,0,0,'2015-01-12 17:30:38'),(23,11,12,4,0,0,'2015-01-12 17:30:39'),(24,12,11,2,0,0,'2015-01-12 17:30:39'),(25,12,13,4,0,0,'2015-01-12 17:30:39'),(26,13,12,2,0,0,'2015-01-12 17:30:39'),(27,13,14,4,0,0,'2015-01-12 17:30:40'),(28,14,13,2,0,0,'2015-01-12 17:30:40'),(29,14,15,4,0,0,'2015-01-12 17:30:40'),(30,15,14,2,0,0,'2015-01-12 17:30:40'),(31,15,16,4,0,0,'2015-01-12 17:30:40'),(32,16,15,2,0,0,'2015-01-12 17:30:40'),(35,16,3,1,0,0,'2015-01-12 17:31:19'),(36,3,16,3,0,0,'2015-01-12 17:31:19'),(37,3,0,4,0,0,'2015-01-12 17:31:30'),(38,0,3,2,0,0,'2015-01-12 17:31:30');
/*!40000 ALTER TABLE `exits` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `load_rules`
--

LOCK TABLES `load_rules` WRITE;
/*!40000 ALTER TABLE `load_rules` DISABLE KEYS */;
/*!40000 ALTER TABLE `load_rules` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `mobs`
--

LOCK TABLES `mobs` WRITE;
/*!40000 ALTER TABLE `mobs` DISABLE KEYS */;
/*!40000 ALTER TABLE `mobs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `notes`
--

LOCK TABLES `notes` WRITE;
/*!40000 ALTER TABLE `notes` DISABLE KEYS */;
/*!40000 ALTER TABLE `notes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `object_instances`
--

LOCK TABLES `object_instances` WRITE;
/*!40000 ALTER TABLE `object_instances` DISABLE KEYS */;
/*!40000 ALTER TABLE `object_instances` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `objects`
--

LOCK TABLES `objects` WRITE;
/*!40000 ALTER TABLE `objects` DISABLE KEYS */;
/*!40000 ALTER TABLE `objects` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `permissions`
--

LOCK TABLES `permissions` WRITE;
/*!40000 ALTER TABLE `permissions` DISABLE KEYS */;
/*!40000 ALTER TABLE `permissions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `player_log`
--

LOCK TABLES `player_log` WRITE;
/*!40000 ALTER TABLE `player_log` DISABLE KEYS */;
INSERT INTO `player_log` VALUES (1,1,5,'Xantiloth (0) logged in from 127.0.0.1','2015-01-14 01:39:22'),(2,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-14 14:23:37'),(3,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-14 14:24:29'),(4,1,5,'Luigi (2) logged in from 127.0.0.1','2015-01-14 16:25:36'),(5,1,5,'Luigi (2) logged in from 127.0.0.1','2015-01-14 16:47:52'),(6,1,5,'17 (0) logged in from 127.0.0.1','2015-01-14 16:47:54'),(7,1,5,'Joe (3) logged in from 127.0.0.1','2015-01-14 16:47:57'),(8,1,5,'Joe (3) logged in from 127.0.0.1','2015-01-14 16:48:01'),(9,1,5,'Luigi (2) logged in from 127.0.0.1','2015-01-14 16:48:07'),(10,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-14 18:26:25'),(11,1,5,'Luigi (2) logged in from 127.0.0.1','2015-01-14 19:21:30'),(12,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-14 19:30:38'),(13,1,5,'Luigi (2) logged in from 127.0.0.1','2015-01-14 19:30:43'),(14,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-14 19:37:05'),(15,1,5,'Luigi (2) logged in from 127.0.0.1','2015-01-14 19:37:09');
/*!40000 ALTER TABLE `player_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `rooms`
--

LOCK TABLES `rooms` WRITE;
/*!40000 ALTER TABLE `rooms` DISABLE KEYS */;
INSERT INTO `rooms` VALUES (1,1,0,'A Frozen Lake','','','','snow',4,'2015-01-12 17:29:44'),(2,1,1,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(3,1,2,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(4,1,3,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(5,1,4,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(6,1,5,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(7,1,6,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(8,1,7,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(9,1,8,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(10,1,9,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(11,1,10,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(12,1,11,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(13,1,12,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(14,1,13,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(15,1,14,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(16,1,15,'Undefined','','','','snow',0,'2015-01-12 17:31:37'),(17,1,16,'Undefined','','','','snow',0,'2015-01-12 17:31:37');
/*!40000 ALTER TABLE `rooms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `socials`
--

LOCK TABLES `socials` WRITE;
/*!40000 ALTER TABLE `socials` DISABLE KEYS */;
/*!40000 ALTER TABLE `socials` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping data for table `world_log`
--

LOCK TABLES `world_log` WRITE;
/*!40000 ALTER TABLE `world_log` DISABLE KEYS */;
INSERT INTO `world_log` VALUES (1,0,1,'System up on port 6501.','2015-01-14 01:39:09'),(2,0,2,'World::save()','2015-01-14 01:39:46'),(3,0,2,'World::save()','2015-01-14 01:41:07'),(4,0,2,'World::save()','2015-01-14 01:41:07'),(5,0,1,'World saved pre-reboot.','2015-01-14 01:41:07'),(6,0,1,'System going down for reboot.','2015-01-14 01:41:07'),(7,0,1,'System rebooted successfully.','2015-01-14 01:41:07'),(8,0,2,'World::save()','2015-01-14 01:44:25'),(9,0,2,'World::save()','2015-01-14 01:44:25'),(10,0,1,'World saved pre-reboot.','2015-01-14 01:44:25'),(11,0,1,'System going down for reboot.','2015-01-14 01:44:25'),(12,0,1,'System rebooted successfully.','2015-01-14 01:44:25'),(13,0,2,'World::save()','2015-01-14 01:45:20'),(14,0,2,'World::save()','2015-01-14 01:45:20'),(15,0,1,'World saved pre-reboot.','2015-01-14 01:45:20'),(16,0,1,'System going down for reboot.','2015-01-14 01:45:20'),(17,0,1,'System rebooted successfully.','2015-01-14 01:45:20'),(18,0,2,'World::save()','2015-01-14 01:47:15'),(19,0,2,'World::save()','2015-01-14 01:47:15'),(20,0,1,'World saved pre-reboot.','2015-01-14 01:47:15'),(21,0,1,'System going down for reboot.','2015-01-14 01:47:15'),(22,0,1,'System rebooted successfully.','2015-01-14 01:47:15'),(23,0,2,'World::save()','2015-01-14 01:59:30'),(24,0,2,'World::save()','2015-01-14 01:59:30'),(25,0,1,'World saved pre-reboot.','2015-01-14 01:59:30'),(26,0,1,'System going down for reboot.','2015-01-14 01:59:30'),(27,0,1,'System rebooted successfully.','2015-01-14 01:59:30'),(28,0,2,'World::save()','2015-01-14 01:59:55'),(29,0,2,'World::save()','2015-01-14 01:59:55'),(30,0,1,'World saved pre-reboot.','2015-01-14 01:59:55'),(31,0,1,'System going down for reboot.','2015-01-14 01:59:55'),(32,0,1,'System rebooted successfully.','2015-01-14 01:59:55'),(33,0,2,'World::save()','2015-01-14 02:00:25'),(34,0,2,'World::save()','2015-01-14 02:00:25'),(35,0,1,'World saved pre-reboot.','2015-01-14 02:00:25'),(36,0,1,'System going down for reboot.','2015-01-14 02:00:25'),(37,0,1,'System rebooted successfully.','2015-01-14 02:00:25'),(38,0,2,'World::save()','2015-01-14 02:01:17'),(39,0,2,'World::save()','2015-01-14 02:01:17'),(40,0,1,'World saved pre-reboot.','2015-01-14 02:01:17'),(41,0,1,'System going down for reboot.','2015-01-14 02:01:17'),(42,0,1,'System rebooted successfully.','2015-01-14 02:01:17'),(43,0,2,'World::save()','2015-01-14 02:02:02'),(44,0,2,'World::save()','2015-01-14 02:02:02'),(45,0,1,'World saved pre-reboot.','2015-01-14 02:02:02'),(46,0,1,'System going down for reboot.','2015-01-14 02:02:02'),(47,0,1,'System rebooted successfully.','2015-01-14 02:02:02'),(48,0,2,'World::save()','2015-01-14 02:02:43'),(49,0,1,'System rebooted successfully.','2015-01-14 14:23:11'),(50,0,1,'System up on port 6501.','2015-01-14 14:23:33'),(51,0,1,'System up on port 6501.','2015-01-14 14:24:18'),(52,0,2,'World::save()','2015-01-14 14:25:27'),(53,0,2,'World::save()','2015-01-14 14:25:27'),(54,0,1,'World saved pre-reboot.','2015-01-14 14:25:27'),(55,0,1,'System going down for reboot.','2015-01-14 14:25:27'),(56,0,1,'System rebooted successfully.','2015-01-14 14:25:27'),(57,0,2,'World::save()','2015-01-14 14:44:36'),(58,0,2,'World::save()','2015-01-14 14:44:36'),(59,0,1,'World saved pre-reboot.','2015-01-14 14:44:36'),(60,0,1,'System going down for reboot.','2015-01-14 14:44:36'),(61,0,1,'System rebooted successfully.','2015-01-14 14:44:36'),(62,0,2,'World::save()','2015-01-14 16:16:01'),(63,0,2,'World::save()','2015-01-14 16:16:01'),(64,0,1,'World saved pre-reboot.','2015-01-14 16:16:01'),(65,0,1,'System going down for reboot.','2015-01-14 16:16:01'),(66,0,1,'System rebooted successfully.','2015-01-14 16:16:02'),(67,0,2,'World::save()','2015-01-14 16:22:21'),(68,0,2,'World::save()','2015-01-14 16:22:21'),(69,0,1,'World saved pre-reboot.','2015-01-14 16:22:21'),(70,0,1,'System going down for reboot.','2015-01-14 16:22:21'),(71,0,1,'System rebooted successfully.','2015-01-14 16:22:21'),(72,0,2,'World::save()','2015-01-14 16:25:28'),(73,0,2,'World::save()','2015-01-14 16:25:28'),(74,0,1,'World saved pre-reboot.','2015-01-14 16:25:28'),(75,0,1,'System going down for reboot.','2015-01-14 16:25:28'),(76,0,1,'System rebooted successfully.','2015-01-14 16:25:28'),(77,0,2,'World::save()','2015-01-14 16:28:47'),(78,0,2,'World::save()','2015-01-14 16:28:47'),(79,0,1,'World saved pre-reboot.','2015-01-14 16:28:47'),(80,0,1,'System going down for reboot.','2015-01-14 16:28:47'),(81,0,1,'System rebooted successfully.','2015-01-14 16:28:47'),(82,0,2,'World::save()','2015-01-14 16:31:05'),(83,0,2,'World::save()','2015-01-14 16:31:05'),(84,0,1,'World saved pre-reboot.','2015-01-14 16:31:05'),(85,0,1,'System going down for reboot.','2015-01-14 16:31:05'),(86,0,1,'System rebooted successfully.','2015-01-14 16:31:05'),(87,0,2,'World::save()','2015-01-14 16:42:40'),(88,0,2,'World::save()','2015-01-14 16:42:40'),(89,0,1,'World saved pre-reboot.','2015-01-14 16:42:40'),(90,0,1,'System going down for reboot.','2015-01-14 16:42:40'),(91,0,1,'System rebooted successfully.','2015-01-14 16:42:40'),(92,0,2,'World::save()','2015-01-14 16:43:17'),(93,0,2,'World::save()','2015-01-14 16:43:17'),(94,0,1,'World saved pre-reboot.','2015-01-14 16:43:17'),(95,0,1,'System going down for reboot.','2015-01-14 16:43:17'),(96,0,1,'System rebooted successfully.','2015-01-14 16:43:17'),(97,0,2,'World::save()','2015-01-14 16:47:08'),(98,0,2,'World::save()','2015-01-14 16:47:08'),(99,0,1,'World saved pre-reboot.','2015-01-14 16:47:08'),(100,0,1,'System going down for reboot.','2015-01-14 16:47:08'),(101,0,1,'System rebooted successfully.','2015-01-14 16:47:08'),(102,0,2,'World::save()','2015-01-14 17:01:41'),(103,0,2,'World::save()','2015-01-14 17:01:41'),(104,0,1,'World saved pre-reboot.','2015-01-14 17:01:41'),(105,0,1,'System going down for reboot.','2015-01-14 17:01:41'),(106,0,1,'System rebooted successfully.','2015-01-14 17:01:41'),(107,0,2,'World::save()','2015-01-14 17:03:17'),(108,0,2,'World::save()','2015-01-14 17:03:17'),(109,0,1,'World saved pre-reboot.','2015-01-14 17:03:17'),(110,0,1,'System going down for reboot.','2015-01-14 17:03:17'),(111,0,1,'System rebooted successfully.','2015-01-14 17:03:17'),(112,0,2,'World::save()','2015-01-14 17:54:15'),(113,0,2,'World::save()','2015-01-14 17:54:15'),(114,0,1,'World saved pre-reboot.','2015-01-14 17:54:15'),(115,0,1,'System going down for reboot.','2015-01-14 17:54:15'),(116,0,1,'System rebooted successfully.','2015-01-14 17:54:15'),(117,0,2,'World::save()','2015-01-14 17:55:06'),(118,0,2,'World::save()','2015-01-14 17:55:06'),(119,0,1,'World saved pre-reboot.','2015-01-14 17:55:06'),(120,0,1,'System going down for reboot.','2015-01-14 17:55:06'),(121,0,1,'System rebooted successfully.','2015-01-14 17:55:06'),(122,0,2,'World::save()','2015-01-14 17:55:34'),(123,0,2,'World::save()','2015-01-14 17:55:34'),(124,0,1,'World saved pre-reboot.','2015-01-14 17:55:34'),(125,0,1,'System going down for reboot.','2015-01-14 17:55:34'),(126,0,1,'System rebooted successfully.','2015-01-14 17:55:34'),(127,0,2,'World::save()','2015-01-14 17:56:33'),(128,0,2,'World::save()','2015-01-14 17:56:33'),(129,0,1,'World saved pre-reboot.','2015-01-14 17:56:33'),(130,0,1,'System going down for reboot.','2015-01-14 17:56:33'),(131,0,1,'System rebooted successfully.','2015-01-14 17:56:34'),(132,0,2,'World::save()','2015-01-14 17:57:21'),(133,0,2,'World::save()','2015-01-14 17:57:21'),(134,0,1,'World saved pre-reboot.','2015-01-14 17:57:21'),(135,0,1,'System going down for reboot.','2015-01-14 17:57:21'),(136,0,1,'System rebooted successfully.','2015-01-14 17:57:21'),(137,0,2,'World::save()','2015-01-14 17:58:13'),(138,0,2,'World::save()','2015-01-14 17:58:13'),(139,0,1,'World saved pre-reboot.','2015-01-14 17:58:13'),(140,0,1,'System going down for reboot.','2015-01-14 17:58:13'),(141,0,1,'System rebooted successfully.','2015-01-14 17:58:13'),(142,0,2,'World::save()','2015-01-14 17:58:40'),(143,0,2,'World::save()','2015-01-14 17:58:40'),(144,0,1,'World saved pre-reboot.','2015-01-14 17:58:40'),(145,0,1,'System going down for reboot.','2015-01-14 17:58:40'),(146,0,1,'System rebooted successfully.','2015-01-14 17:58:40'),(147,0,2,'World::save()','2015-01-14 17:59:20'),(148,0,2,'World::save()','2015-01-14 17:59:20'),(149,0,1,'World saved pre-reboot.','2015-01-14 17:59:20'),(150,0,1,'System going down for reboot.','2015-01-14 17:59:20'),(151,0,1,'System rebooted successfully.','2015-01-14 17:59:20'),(152,0,2,'World::save()','2015-01-14 17:59:26'),(153,0,2,'World::save()','2015-01-14 17:59:26'),(154,0,1,'World saved pre-reboot.','2015-01-14 17:59:26'),(155,0,1,'System going down for reboot.','2015-01-14 17:59:26'),(156,0,1,'System rebooted successfully.','2015-01-14 17:59:26'),(157,0,2,'World::save()','2015-01-14 18:10:04'),(158,0,2,'World::save()','2015-01-14 18:10:04'),(159,0,1,'World saved pre-reboot.','2015-01-14 18:10:04'),(160,0,1,'System going down for reboot.','2015-01-14 18:10:04'),(161,0,1,'System rebooted successfully.','2015-01-14 18:10:04'),(162,0,2,'World::save()','2015-01-14 18:10:59'),(163,0,2,'World::save()','2015-01-14 18:10:59'),(164,0,1,'World saved pre-reboot.','2015-01-14 18:10:59'),(165,0,1,'System going down for reboot.','2015-01-14 18:10:59'),(166,0,1,'System rebooted successfully.','2015-01-14 18:10:59'),(167,0,2,'World::save()','2015-01-14 18:12:25'),(168,0,2,'World::save()','2015-01-14 18:12:25'),(169,0,1,'World saved pre-reboot.','2015-01-14 18:12:25'),(170,0,1,'System going down for reboot.','2015-01-14 18:12:25'),(171,0,1,'System rebooted successfully.','2015-01-14 18:12:25'),(172,0,2,'World::save()','2015-01-14 18:13:21'),(173,0,2,'World::save()','2015-01-14 18:13:21'),(174,0,1,'World saved pre-reboot.','2015-01-14 18:13:21'),(175,0,1,'System going down for reboot.','2015-01-14 18:13:21'),(176,0,1,'System rebooted successfully.','2015-01-14 18:13:21'),(177,0,1,'System up on port 6501.','2015-01-14 18:26:22'),(178,0,2,'World::save()','2015-01-14 18:38:28'),(179,0,2,'World::save()','2015-01-14 18:38:28'),(180,0,1,'World saved pre-reboot.','2015-01-14 18:38:28'),(181,0,1,'System going down for reboot.','2015-01-14 18:38:28'),(182,0,1,'System rebooted successfully.','2015-01-14 18:38:28'),(183,0,2,'World::save()','2015-01-14 18:39:29'),(184,0,2,'World::save()','2015-01-14 18:39:29'),(185,0,1,'World saved pre-reboot.','2015-01-14 18:39:29'),(186,0,1,'System going down for reboot.','2015-01-14 18:39:29'),(187,0,1,'System rebooted successfully.','2015-01-14 18:39:29'),(188,0,2,'World::save()','2015-01-14 18:47:38'),(189,0,2,'World::save()','2015-01-14 18:47:38'),(190,0,1,'World saved pre-reboot.','2015-01-14 18:47:38'),(191,0,1,'System going down for reboot.','2015-01-14 18:47:38'),(192,0,1,'System rebooted successfully.','2015-01-14 18:47:38'),(193,0,2,'World::save()','2015-01-14 18:48:36'),(194,0,2,'World::save()','2015-01-14 18:48:36'),(195,0,1,'World saved pre-reboot.','2015-01-14 18:48:36'),(196,0,1,'System going down for reboot.','2015-01-14 18:48:36'),(197,0,1,'System rebooted successfully.','2015-01-14 18:48:36'),(198,0,2,'World::save()','2015-01-14 18:51:06'),(199,0,2,'World::save()','2015-01-14 18:51:06'),(200,0,1,'World saved pre-reboot.','2015-01-14 18:51:06'),(201,0,1,'System going down for reboot.','2015-01-14 18:51:06'),(202,0,1,'System rebooted successfully.','2015-01-14 18:51:06'),(203,0,2,'World::save()','2015-01-14 19:21:21'),(204,0,2,'World::save()','2015-01-14 19:21:21'),(205,0,1,'World saved pre-reboot.','2015-01-14 19:21:21'),(206,0,1,'System going down for reboot.','2015-01-14 19:21:21'),(207,0,1,'System rebooted successfully.','2015-01-14 19:21:21'),(208,0,2,'World::save()','2015-01-14 19:21:41'),(209,0,2,'World::save()','2015-01-14 19:21:41'),(210,0,1,'World saved pre-reboot.','2015-01-14 19:21:41'),(211,0,1,'System going down for reboot.','2015-01-14 19:21:41'),(212,0,1,'System rebooted successfully.','2015-01-14 19:21:41'),(213,0,2,'World::save()','2015-01-14 19:30:19'),(214,0,2,'World::save()','2015-01-14 19:30:19'),(215,0,1,'World saved pre-reboot.','2015-01-14 19:30:19'),(216,0,1,'System going down for reboot.','2015-01-14 19:30:19'),(217,0,1,'System rebooted successfully.','2015-01-14 19:30:19'),(218,0,1,'System up on port 6501.','2015-01-14 19:30:35'),(219,0,2,'World::save()','2015-01-14 19:34:59'),(220,0,2,'World::save()','2015-01-14 19:34:59'),(221,0,1,'World saved pre-reboot.','2015-01-14 19:34:59'),(222,0,1,'System going down for reboot.','2015-01-14 19:34:59'),(223,0,1,'System rebooted successfully.','2015-01-14 19:35:00'),(224,0,1,'System up on port 6501.','2015-01-14 19:36:58'),(225,0,2,'World::save()','2015-01-14 19:37:16'),(226,0,2,'World::save()','2015-01-14 19:42:24'),(227,0,2,'World::save()','2015-01-14 19:42:24'),(228,0,1,'World saved pre-reboot.','2015-01-14 19:42:24'),(229,0,1,'System going down for reboot.','2015-01-14 19:42:24'),(230,0,1,'System rebooted successfully.','2015-01-14 19:42:25'),(231,0,2,'World::save()','2015-01-14 19:43:01'),(232,0,2,'World::save()','2015-01-14 19:43:01'),(233,0,1,'World saved pre-reboot.','2015-01-14 19:43:01'),(234,0,1,'System going down for reboot.','2015-01-14 19:43:01'),(235,0,1,'System rebooted successfully.','2015-01-14 19:43:02'),(236,0,2,'World::save()','2015-01-14 19:43:31'),(237,0,2,'World::save()','2015-01-14 19:43:32'),(238,0,1,'World saved pre-reboot.','2015-01-14 19:43:32'),(239,0,1,'System going down for reboot.','2015-01-14 19:43:32'),(240,0,1,'System rebooted successfully.','2015-01-14 19:43:32'),(241,0,2,'World::save()','2015-01-14 19:46:24'),(242,0,2,'World::save()','2015-01-14 19:46:24'),(243,0,1,'World saved pre-reboot.','2015-01-14 19:46:24'),(244,0,1,'System going down for reboot.','2015-01-14 19:46:24'),(245,0,1,'System rebooted successfully.','2015-01-14 19:46:25'),(246,0,2,'World::save()','2015-01-14 19:48:53'),(247,0,2,'World::save()','2015-01-14 19:48:53'),(248,0,1,'World saved pre-reboot.','2015-01-14 19:48:53'),(249,0,1,'System going down for reboot.','2015-01-14 19:48:53'),(250,0,1,'System rebooted successfully.','2015-01-14 19:48:54'),(251,0,2,'World::save()','2015-01-14 19:49:44'),(252,0,2,'World::save()','2015-01-14 19:49:44'),(253,0,1,'World saved pre-reboot.','2015-01-14 19:49:44'),(254,0,1,'System going down for reboot.','2015-01-14 19:49:44'),(255,0,1,'System rebooted successfully.','2015-01-14 19:49:45'),(256,0,2,'World::save()','2015-01-14 19:50:04'),(257,0,2,'World::save()','2015-01-14 19:50:04'),(258,0,1,'World saved pre-reboot.','2015-01-14 19:50:04'),(259,0,1,'System going down for reboot.','2015-01-14 19:50:04'),(260,0,1,'System rebooted successfully.','2015-01-14 19:50:05');
/*!40000 ALTER TABLE `world_log` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-01-14 19:52:05
