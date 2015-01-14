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
INSERT INTO `abilities` VALUES (1,'second strike',25,'2015-01-14 01:47:15');
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
INSERT INTO `avatars` VALUES (1,1,0,'Xantiloth','','*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19',1,3,4,0,125,0,0,6097,6097,472,472,1551,1551,17,22,16,21,17,22,13,18,13,18,15,20,100,0,0,0,0,17,0,0,0,0,0,'x',0,0,0,', the Absent Minded','','','2015-01-14 01:47:15',15,20,15,20,127),(2,1,0,'Luigi','','*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19',1,2,3,0,25,0,2969,268,268,184,184,206,206,15,20,19,24,13,18,17,22,15,20,15,20,100,0,0,0,0,17,0,0,0,0,0,'x',0,0,0,', adventurer.','','','2015-01-14 01:47:04',16,21,15,20,29);
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
INSERT INTO `player_log` VALUES (1,1,5,'Xantiloth (0) logged in from 127.0.0.1','2015-01-14 01:39:22');
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
INSERT INTO `world_log` VALUES (1,0,1,'System up on port 6501.','2015-01-14 01:39:09'),(2,0,2,'World::save()','2015-01-14 01:39:46'),(3,0,2,'World::save()','2015-01-14 01:41:07'),(4,0,2,'World::save()','2015-01-14 01:41:07'),(5,0,1,'World saved pre-reboot.','2015-01-14 01:41:07'),(6,0,1,'System going down for reboot.','2015-01-14 01:41:07'),(7,0,1,'System rebooted successfully.','2015-01-14 01:41:07'),(8,0,2,'World::save()','2015-01-14 01:44:25'),(9,0,2,'World::save()','2015-01-14 01:44:25'),(10,0,1,'World saved pre-reboot.','2015-01-14 01:44:25'),(11,0,1,'System going down for reboot.','2015-01-14 01:44:25'),(12,0,1,'System rebooted successfully.','2015-01-14 01:44:25'),(13,0,2,'World::save()','2015-01-14 01:45:20'),(14,0,2,'World::save()','2015-01-14 01:45:20'),(15,0,1,'World saved pre-reboot.','2015-01-14 01:45:20'),(16,0,1,'System going down for reboot.','2015-01-14 01:45:20'),(17,0,1,'System rebooted successfully.','2015-01-14 01:45:20'),(18,0,2,'World::save()','2015-01-14 01:47:15'),(19,0,2,'World::save()','2015-01-14 01:47:15'),(20,0,1,'World saved pre-reboot.','2015-01-14 01:47:15'),(21,0,1,'System going down for reboot.','2015-01-14 01:47:15'),(22,0,1,'System rebooted successfully.','2015-01-14 01:47:15');
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

-- Dump completed on 2015-01-14  1:47:35
