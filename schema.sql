-- MySQL dump 10.13  Distrib 8.4.8, for Linux (x86_64)
--
-- Host: localhost    Database: SIMLIPAL
-- ------------------------------------------------------
-- Server version	8.4.8-0ubuntu0.25.10.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `ACTIVITY_LOG`
--

DROP TABLE IF EXISTS `ACTIVITY_LOG`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ACTIVITY_LOG` (
  `log_id` int NOT NULL AUTO_INCREMENT,
  `user_id` int NOT NULL,
  `action` varchar(20) NOT NULL,
  `entity` varchar(50) NOT NULL,
  `entity_id` int DEFAULT NULL,
  `field_name` varchar(50) DEFAULT NULL,
  `old_value` text,
  `created_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`log_id`)
) ENGINE=InnoDB AUTO_INCREMENT=332 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ADMIN`
--

DROP TABLE IF EXISTS `ADMIN`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ADMIN` (
  `user_id` int NOT NULL AUTO_INCREMENT,
  `email` varchar(255) NOT NULL,
  `role` enum('ADMIN','DBA','USER') NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `email` (`email`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `BRAND`
--

DROP TABLE IF EXISTS `BRAND`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `BRAND` (
  `brand_id` int NOT NULL AUTO_INCREMENT,
  `brand_name` varchar(60) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  `psychographics` text,
  `genre_description` text,
  `company_type` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`brand_id`),
  UNIQUE KEY `brand_name` (`brand_name`)
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `BRAND_GENRE`
--

DROP TABLE IF EXISTS `BRAND_GENRE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `BRAND_GENRE` (
  `brand_id` int NOT NULL,
  `genre_id` int NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`brand_id`,`genre_id`),
  UNIQUE KEY `unique_brand_genre` (`brand_id`,`genre_id`),
  KEY `genre_id` (`genre_id`),
  CONSTRAINT `BRAND_GENRE_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`) ON DELETE CASCADE,
  CONSTRAINT `BRAND_GENRE_ibfk_2` FOREIGN KEY (`genre_id`) REFERENCES `GENRE` (`genre_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `BRAND_STANDARD`
--

DROP TABLE IF EXISTS `BRAND_STANDARD`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `BRAND_STANDARD` (
  `standard_id` int NOT NULL AUTO_INCREMENT,
  `brand_id` int NOT NULL,
  `position` text,
  `brand_identity` text,
  `csr` text,
  `quality` text,
  `perception` text,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`standard_id`),
  UNIQUE KEY `brand_id` (`brand_id`),
  CONSTRAINT `BRAND_STANDARD_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `BRAND_TARGET_AUDIENCE`
--

DROP TABLE IF EXISTS `BRAND_TARGET_AUDIENCE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `BRAND_TARGET_AUDIENCE` (
  `brand_id` int NOT NULL,
  `aud_id` int NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`brand_id`,`aud_id`),
  UNIQUE KEY `unique_brand_audience` (`brand_id`,`aud_id`),
  KEY `aud_id` (`aud_id`),
  CONSTRAINT `BRAND_TARGET_AUDIENCE_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`) ON DELETE CASCADE,
  CONSTRAINT `BRAND_TARGET_AUDIENCE_ibfk_2` FOREIGN KEY (`aud_id`) REFERENCES `TARGET_AUDIENCE` (`aud_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `B_POSITION`
--

DROP TABLE IF EXISTS `B_POSITION`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `B_POSITION` (
  `position_id` int NOT NULL AUTO_INCREMENT,
  `position_name` varchar(100) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`position_id`),
  UNIQUE KEY `position_name` (`position_name`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `COMMUNITY_REACHES`
--

DROP TABLE IF EXISTS `COMMUNITY_REACHES`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `COMMUNITY_REACHES` (
  `comm_id` int NOT NULL AUTO_INCREMENT,
  `creator_id` int NOT NULL,
  `sentiment` text,
  `reply_rate` text,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`comm_id`),
  UNIQUE KEY `creator_id` (`creator_id`),
  CONSTRAINT `COMMUNITY_REACHES_ibfk_1` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `COMPETITORS`
--

DROP TABLE IF EXISTS `COMPETITORS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `COMPETITORS` (
  `competitors_id` int NOT NULL AUTO_INCREMENT,
  `brand_id` int NOT NULL,
  `name` varchar(100) NOT NULL,
  `type` text,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`competitors_id`),
  UNIQUE KEY `brand_id` (`brand_id`,`name`),
  CONSTRAINT `COMPETITORS_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `CREATOR`
--

DROP TABLE IF EXISTS `CREATOR`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `CREATOR` (
  `creator_id` int NOT NULL AUTO_INCREMENT,
  `creator_name` varchar(255) NOT NULL,
  `email_id` varchar(255) DEFAULT NULL,
  `rating` varchar(255) DEFAULT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  `audience_size` bigint DEFAULT NULL,
  PRIMARY KEY (`creator_id`),
  UNIQUE KEY `email_id` (`email_id`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `CREATOR_NICHE`
--

DROP TABLE IF EXISTS `CREATOR_NICHE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `CREATOR_NICHE` (
  `creator_id` int NOT NULL,
  `niche_id` int NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`creator_id`,`niche_id`),
  UNIQUE KEY `unique_creator_niche` (`creator_id`,`niche_id`),
  KEY `niche_id` (`niche_id`),
  CONSTRAINT `CREATOR_NICHE_ibfk_1` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`) ON DELETE CASCADE,
  CONSTRAINT `CREATOR_NICHE_ibfk_2` FOREIGN KEY (`niche_id`) REFERENCES `NICHE` (`niche_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `CREATOR_TARGET_AUDIENCE`
--

DROP TABLE IF EXISTS `CREATOR_TARGET_AUDIENCE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `CREATOR_TARGET_AUDIENCE` (
  `creator_id` int NOT NULL,
  `aud_id` int NOT NULL,
  `is_deleted` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`creator_id`,`aud_id`),
  KEY `aud_id` (`aud_id`),
  CONSTRAINT `CREATOR_TARGET_AUDIENCE_ibfk_1` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`),
  CONSTRAINT `CREATOR_TARGET_AUDIENCE_ibfk_2` FOREIGN KEY (`aud_id`) REFERENCES `TARGET_AUDIENCE` (`aud_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `DEAL`
--

DROP TABLE IF EXISTS `DEAL`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `DEAL` (
  `deal_id` int NOT NULL AUTO_INCREMENT,
  `brand_id` int DEFAULT NULL,
  `creator_id` int DEFAULT NULL,
  `is_lead` tinyint DEFAULT '0',
  `is_connected` tinyint DEFAULT '0',
  `is_deal_done` tinyint DEFAULT '0',
  `post_uploaded` tinyint DEFAULT '0',
  `payment_cleared` tinyint DEFAULT '0',
  `date` date DEFAULT NULL,
  `is_deleted` tinyint DEFAULT '0',
  PRIMARY KEY (`deal_id`),
  KEY `brand_id` (`brand_id`),
  KEY `creator_id` (`creator_id`),
  CONSTRAINT `DEAL_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`),
  CONSTRAINT `DEAL_ibfk_2` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `GENRE`
--

DROP TABLE IF EXISTS `GENRE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `GENRE` (
  `genre_id` int NOT NULL AUTO_INCREMENT,
  `category` varchar(100) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`genre_id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `GOALS`
--

DROP TABLE IF EXISTS `GOALS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `GOALS` (
  `goal_id` int NOT NULL AUTO_INCREMENT,
  `brand_id` int NOT NULL,
  `description` text,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`goal_id`),
  KEY `GOALS_ibfk_1` (`brand_id`),
  CONSTRAINT `GOALS_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `HEADS`
--

DROP TABLE IF EXISTS `HEADS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `HEADS` (
  `head_id` int NOT NULL AUTO_INCREMENT,
  `brand_id` int NOT NULL,
  `position_id` int NOT NULL,
  `name` varchar(100) NOT NULL,
  `linkedin_id` varchar(50) DEFAULT NULL,
  `personal_mail` varchar(255) NOT NULL,
  `official_mail` varchar(255) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`head_id`),
  UNIQUE KEY `personal_mail` (`personal_mail`),
  UNIQUE KEY `official_mail` (`official_mail`),
  UNIQUE KEY `brand_id` (`brand_id`,`name`),
  KEY `position_id` (`position_id`),
  CONSTRAINT `HEADS_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`) ON DELETE CASCADE,
  CONSTRAINT `HEADS_ibfk_2` FOREIGN KEY (`position_id`) REFERENCES `B_POSITION` (`position_id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `HEAD_CONTACT`
--

DROP TABLE IF EXISTS `HEAD_CONTACT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `HEAD_CONTACT` (
  `contacts_id` int NOT NULL AUTO_INCREMENT,
  `head_id` int NOT NULL,
  `contact_type` varchar(50) DEFAULT NULL,
  `contact_value` varchar(25) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`contacts_id`),
  UNIQUE KEY `head_id` (`head_id`,`contact_value`),
  CONSTRAINT `HEAD_CONTACT_ibfk_1` FOREIGN KEY (`head_id`) REFERENCES `HEADS` (`head_id`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `HPV_AUDIT`
--

DROP TABLE IF EXISTS `HPV_AUDIT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `HPV_AUDIT` (
  `hpv_id` int NOT NULL AUTO_INCREMENT,
  `creator_id` int NOT NULL,
  `topic` varchar(255) DEFAULT NULL,
  `spike_rate` varchar(255) DEFAULT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`hpv_id`),
  KEY `creator_id` (`creator_id`),
  CONSTRAINT `HPV_AUDIT_ibfk_1` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `INSIGHTS`
--

DROP TABLE IF EXISTS `INSIGHTS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `INSIGHTS` (
  `insights_id` int NOT NULL AUTO_INCREMENT,
  `brand_id` int NOT NULL,
  `pitch_angle` text,
  `leverage` text,
  `gap` text,
  `hook` text,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`insights_id`),
  UNIQUE KEY `brand_id` (`brand_id`),
  CONSTRAINT `INSIGHTS_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `LEVERAGE_DET`
--

DROP TABLE IF EXISTS `LEVERAGE_DET`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `LEVERAGE_DET` (
  `leverage_id` int NOT NULL AUTO_INCREMENT,
  `creator_id` int NOT NULL,
  `curr_sponsors` varchar(255) DEFAULT NULL,
  `undervalued` varchar(255) DEFAULT NULL,
  `missed_verticals` varchar(255) DEFAULT NULL,
  `content_gaps` varchar(255) DEFAULT NULL,
  `language_leverage` varchar(255) DEFAULT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`leverage_id`),
  UNIQUE KEY `creator_id` (`creator_id`),
  CONSTRAINT `LEVERAGE_DET_ibfk_1` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `LOGIN`
--

DROP TABLE IF EXISTS `LOGIN`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `LOGIN` (
  `login_id` int NOT NULL AUTO_INCREMENT,
  `user_id` int NOT NULL,
  `password_hash` varchar(255) NOT NULL,
  `is_active` tinyint(1) NOT NULL DEFAULT '1',
  `created_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `last_login_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`login_id`),
  KEY `fk_login_user` (`user_id`),
  CONSTRAINT `fk_login_user` FOREIGN KEY (`user_id`) REFERENCES `ADMIN` (`user_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `NEWS_FUNDING`
--

DROP TABLE IF EXISTS `NEWS_FUNDING`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `NEWS_FUNDING` (
  `news_id` int NOT NULL AUTO_INCREMENT,
  `brand_id` int NOT NULL,
  `event_type` text,
  `title` varchar(100) DEFAULT NULL,
  `description` text,
  `event_date` date DEFAULT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`news_id`),
  KEY `NEWS_FUNDING_ibfk_1` (`brand_id`),
  CONSTRAINT `NEWS_FUNDING_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `NICHE`
--

DROP TABLE IF EXISTS `NICHE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `NICHE` (
  `niche_id` int NOT NULL AUTO_INCREMENT,
  `niche_type` varchar(100) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`niche_id`),
  UNIQUE KEY `niche_type` (`niche_type`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `PAST_INFLUENCER_CAMP`
--

DROP TABLE IF EXISTS `PAST_INFLUENCER_CAMP`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `PAST_INFLUENCER_CAMP` (
  `p_inf_id` int NOT NULL AUTO_INCREMENT,
  `brand_id` int DEFAULT NULL,
  `ambassadors` text,
  `campaigns` text,
  `strategy` text,
  `is_deleted` tinyint DEFAULT '0',
  PRIMARY KEY (`p_inf_id`),
  KEY `brand_id` (`brand_id`),
  CONSTRAINT `PAST_INFLUENCER_CAMP_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `PLATFORM`
--

DROP TABLE IF EXISTS `PLATFORM`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `PLATFORM` (
  `platform_id` int NOT NULL AUTO_INCREMENT,
  `platform_name` varchar(30) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`platform_id`),
  UNIQUE KEY `platform_name` (`platform_name`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `PROFILE_ACCOUNT`
--

DROP TABLE IF EXISTS `PROFILE_ACCOUNT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `PROFILE_ACCOUNT` (
  `profile_id` int NOT NULL AUTO_INCREMENT,
  `link` varchar(255) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  `creator_id` int NOT NULL,
  `platform_id` int NOT NULL,
  `followers` bigint DEFAULT '0',
  PRIMARY KEY (`profile_id`),
  UNIQUE KEY `uq_creator_platform_link` (`creator_id`,`platform_id`,`link`),
  KEY `fk_profile_platform` (`platform_id`),
  CONSTRAINT `fk_profile_creator` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`),
  CONSTRAINT `fk_profile_platform` FOREIGN KEY (`platform_id`) REFERENCES `PLATFORM` (`platform_id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `REVENUE`
--

DROP TABLE IF EXISTS `REVENUE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `REVENUE` (
  `revenue_id` int NOT NULL AUTO_INCREMENT,
  `brand_id` int NOT NULL,
  `FinancialYear` int DEFAULT NULL,
  `revenue_period` varchar(20) DEFAULT NULL,
  `RevenueSource` varchar(100) DEFAULT NULL,
  `revenue_type` varchar(50) DEFAULT NULL,
  `revenue_value` decimal(15,2) NOT NULL,
  `currency` varchar(50) DEFAULT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`revenue_id`),
  UNIQUE KEY `brand_id` (`brand_id`,`FinancialYear`,`revenue_period`),
  CONSTRAINT `REVENUE_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `BRAND` (`brand_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `STRATEGIC_NOTE`
--

DROP TABLE IF EXISTS `STRATEGIC_NOTE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `STRATEGIC_NOTE` (
  `strategic_id` int NOT NULL AUTO_INCREMENT,
  `creator_id` int NOT NULL,
  `description` text,
  `author_name` varchar(100) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`strategic_id`),
  KEY `creator_id` (`creator_id`),
  CONSTRAINT `STRATEGIC_NOTE_ibfk_1` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TARGET_AUDIENCE`
--

DROP TABLE IF EXISTS `TARGET_AUDIENCE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `TARGET_AUDIENCE` (
  `aud_id` int NOT NULL AUTO_INCREMENT,
  `aud_name` varchar(100) NOT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`aud_id`),
  UNIQUE KEY `aud_name` (`aud_name`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `VERDICT`
--

DROP TABLE IF EXISTS `VERDICT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `VERDICT` (
  `verdict_id` int NOT NULL AUTO_INCREMENT,
  `creator_id` int NOT NULL,
  `strengths` varchar(255) DEFAULT NULL,
  `weaknesses` varchar(255) DEFAULT NULL,
  `growth_potential` varchar(255) DEFAULT NULL,
  `final_verdict` varchar(255) DEFAULT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`verdict_id`),
  UNIQUE KEY `creator_id` (`creator_id`),
  CONSTRAINT `VERDICT_ibfk_1` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `VIDEO_L10`
--

DROP TABLE IF EXISTS `VIDEO_L10`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `VIDEO_L10` (
  `performance_id` int NOT NULL AUTO_INCREMENT,
  `creator_id` int NOT NULL,
  `L10_consistency` varchar(255) DEFAULT NULL,
  `L10_avg_views` varchar(255) DEFAULT NULL,
  `L10_avg_ER` varchar(255) DEFAULT NULL,
  `L10_sharability` varchar(255) DEFAULT NULL,
  `velocity_trend` varchar(255) DEFAULT NULL,
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`performance_id`),
  KEY `creator_id` (`creator_id`),
  CONSTRAINT `VIDEO_L10_ibfk_1` FOREIGN KEY (`creator_id`) REFERENCES `CREATOR` (`creator_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-05-21  0:42:50
