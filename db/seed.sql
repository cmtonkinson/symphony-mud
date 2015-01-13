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
INSERT INTO `abilities` VALUES (1,'fourth strike',80,'2015-01-13 15:04:27'),(1,'second strike',100,'2015-01-13 15:04:27'),(1,'third strike',90,'2015-01-13 15:04:27');
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
INSERT INTO `avatars` VALUES (1,1,0,'Xantiloth','','*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19',1,3,4,0,125,0,0,6097,6097,472,472,1551,1551,17,22,16,21,17,22,13,18,13,18,15,20,100,0,0,0,0,0,5,0,17,0,0,0,0,0,0,'x',0,0,3,', the Absent Minded','','','2015-01-12 17:47:43',15,20,15,20),(2,1,0,'Luigi','','*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19',1,2,3,0,75,0,48283,2041,2041,435,435,710,710,15,20,19,24,13,18,17,22,15,20,15,20,100,0,0,0,0,0,5,31868,17,0,0,0,0,0,0,'x',0,0,3,', adventurer.','','','2015-01-12 17:47:43',16,21,15,20),(4,1,0,'John','','*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19',1,1,1,0,25,0,2969,249,249,249,249,206,206,16,21,14,19,17,22,15,20,17,22,14,19,100,0,0,0,0,0,5,0,17,0,0,0,0,0,0,'x',0,0,3,', adventurer.','','','2015-01-12 17:47:43',14,19,18,23),(5,1,0,'Joe','','*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19',1,2,2,0,50,0,17254,597,597,764,764,405,405,13,18,17,22,12,17,18,23,17,22,17,22,100,0,0,0,0,0,5,46,17,0,0,0,0,0,0,'x',0,0,3,', adventurer.','','','2015-01-12 23:18:07',17,22,15,20);
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
INSERT INTO `player_log` VALUES (1,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-12 17:10:31'),(2,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-12 17:11:03'),(3,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-12 17:11:48'),(4,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-12 17:13:44'),(5,1,5,'Luigi (2) logged in from 127.0.0.1','2015-01-12 17:13:46'),(6,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-12 17:14:20'),(7,1,5,'Luigi (2) logged in from 127.0.0.1','2015-01-12 17:14:34'),(8,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-12 17:25:07'),(9,1,5,'Joe (3) logged in from 127.0.0.1','2015-01-12 17:48:45'),(10,1,5,'Joe (3) logged in from 127.0.0.1','2015-01-12 17:48:50'),(11,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-13 14:27:36'),(12,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-13 14:27:44'),(13,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-13 14:30:00'),(14,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-13 14:30:07'),(15,1,5,'Xantiloth (1) logged in from 127.0.0.1','2015-01-13 14:53:57'),(16,1,5,'Luigi (2) logged in from 127.0.0.1','2015-01-13 14:54:02'),(17,1,5,'Joe (5) logged in from 127.0.0.1','2015-01-13 14:54:10'),(18,1,5,'John (4) logged in from 127.0.0.1','2015-01-13 14:54:14');
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
INSERT INTO `world_log` VALUES (1,0,1,'System up on port 6501.','2015-01-12 17:09:43'),(2,0,1,'System up on port 6501.','2015-01-12 17:10:27'),(3,0,1,'System up on port 6501.','2015-01-12 17:10:49'),(4,0,1,'System up on port 6501.','2015-01-12 17:11:42'),(5,0,1,'System up on port 6501.','2015-01-12 17:14:05'),(6,0,1,'System up on port 6501.','2015-01-12 17:24:25'),(7,0,2,'World::save()','2015-01-12 17:29:44'),(8,0,2,'World::save()','2015-01-12 17:31:37'),(9,0,2,'World::save()','2015-01-12 17:31:40'),(10,0,2,'World::save()','2015-01-12 17:31:40'),(11,0,1,'World saved pre-reboot.','2015-01-12 17:31:40'),(12,0,1,'System going down for reboot.','2015-01-12 17:31:40'),(13,0,1,'System rebooted successfully.','2015-01-12 17:31:40'),(14,0,2,'World::save()','2015-01-12 17:36:28'),(15,0,2,'World::save()','2015-01-12 17:36:30'),(16,0,2,'World::save()','2015-01-12 17:36:30'),(17,0,1,'World saved pre-reboot.','2015-01-12 17:36:30'),(18,0,1,'System going down for reboot.','2015-01-12 17:36:30'),(19,0,1,'System rebooted successfully.','2015-01-12 17:36:30'),(20,0,2,'World::save()','2015-01-12 17:47:43'),(21,0,2,'World::save()','2015-01-12 23:18:07'),(22,0,2,'World::save()','2015-01-12 23:18:07'),(23,0,1,'World saved pre-reboot.','2015-01-12 23:18:07'),(24,0,1,'System going down for reboot.','2015-01-12 23:18:07'),(25,0,1,'System rebooted successfully.','2015-01-12 23:18:08'),(26,0,1,'System rebooted successfully.','2015-01-13 14:26:17'),(27,0,2,'World::save()','2015-01-13 14:27:25'),(28,0,2,'World::save()','2015-01-13 14:27:25'),(29,0,1,'World saved pre-reboot.','2015-01-13 14:27:25'),(30,0,1,'System going down for reboot.','2015-01-13 14:27:25'),(31,0,1,'System rebooted successfully.','2015-01-13 14:27:25'),(32,0,2,'World::save()','2015-01-13 14:27:51'),(33,0,2,'World::save()','2015-01-13 14:27:51'),(34,0,1,'World saved pre-reboot.','2015-01-13 14:27:51'),(35,0,1,'System going down for reboot.','2015-01-13 14:27:51'),(36,0,1,'System rebooted successfully.','2015-01-13 14:27:51'),(37,0,2,'World::save()','2015-01-13 14:29:35'),(38,0,2,'World::save()','2015-01-13 14:29:35'),(39,0,1,'World saved pre-reboot.','2015-01-13 14:29:35'),(40,0,1,'System going down for reboot.','2015-01-13 14:29:35'),(41,0,1,'System rebooted successfully.','2015-01-13 14:29:35'),(42,0,2,'World::save()','2015-01-13 14:30:16'),(43,0,2,'World::save()','2015-01-13 14:30:16'),(44,0,1,'World saved pre-reboot.','2015-01-13 14:30:16'),(45,0,1,'System going down for reboot.','2015-01-13 14:30:16'),(46,0,1,'System rebooted successfully.','2015-01-13 14:30:16'),(47,0,2,'World::save()','2015-01-13 14:52:06'),(48,0,2,'World::save()','2015-01-13 14:52:06'),(49,0,1,'World saved pre-reboot.','2015-01-13 14:52:06'),(50,0,1,'System going down for reboot.','2015-01-13 14:52:06'),(51,0,1,'System rebooted successfully.','2015-01-13 14:52:06'),(52,0,2,'World::save()','2015-01-13 14:53:03'),(53,0,2,'World::save()','2015-01-13 14:53:03'),(54,0,1,'World saved pre-reboot.','2015-01-13 14:53:03'),(55,0,1,'System going down for reboot.','2015-01-13 14:53:03'),(56,0,1,'System rebooted successfully.','2015-01-13 14:53:03'),(57,0,1,'System up on port 6501.','2015-01-13 14:53:52'),(58,0,2,'World::save()','2015-01-13 14:54:27'),(59,0,2,'World::save()','2015-01-13 14:54:27'),(60,0,1,'World saved pre-reboot.','2015-01-13 14:54:27'),(61,0,1,'System going down for reboot.','2015-01-13 14:54:27'),(62,0,1,'System rebooted successfully.','2015-01-13 14:54:27'),(63,0,2,'World::save()','2015-01-13 15:04:27'),(64,0,2,'World::save()','2015-01-13 15:04:27'),(65,0,1,'World saved pre-reboot.','2015-01-13 15:04:27'),(66,0,1,'System going down for reboot.','2015-01-13 15:04:27'),(67,0,1,'System rebooted successfully.','2015-01-13 15:04:27');
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

-- Dump completed on 2015-01-13 15:12:57
