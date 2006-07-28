-- phpMyAdmin SQL Dump
-- version 2.7.0-pl2
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Generation Time: Apr 03, 2006 at 04:36 PM
-- Server version: 4.1.12
-- PHP Version: 4.4.0
-- 
-- Database: `phpticket`
-- 

-- --------------------------------------------------------

-- 
-- Table structure for table `action`
-- 

CREATE TABLE `action` (
  `id` int(8) NOT NULL auto_increment,
  `ticket_id` int(8) NOT NULL default '0',
  `t_date` datetime default NULL,
  `description` text NOT NULL,
  `user` int(8) default NULL,
  `action_type` int(8) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `ID` (`id`),
  KEY `ID_2` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 PACK_KEYS=0 AUTO_INCREMENT=3 ;

-- 
-- Dumping data for table `action`
-- 

INSERT INTO `action` VALUES (1, 1, '2002-11-25 16:26:10', 'This is a test', 1, 10);
INSERT INTO `action` VALUES (2, 2, '2005-09-12 08:49:28', 'new action', 2, 1);


-- --------------------------------------------------------

-- 
-- Table structure for table `notify`
-- 

CREATE TABLE `notify` (
  `id` int(8) NOT NULL auto_increment,
  `ticket_id` int(8) NOT NULL default '0',
  `user` int(8) NOT NULL default '0',
  `execute_path` tinytext,
  `on_action` tinyint(1) default '0',
  `on_ticket` tinyint(1) default '0',
  `email_address` tinytext,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `ID` (`id`),
  KEY `ID_2` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

-- 
-- Dumping data for table `notify`
-- 

INSERT INTO `notify` VALUES (1, 1, 1, '', 1, 1, 'your.name@your.company.com');

-- --------------------------------------------------------

-- 
-- Table structure for table `options`
-- 

CREATE TABLE `options` (
  `id` int(4) NOT NULL auto_increment,
  `orden` tinyint(4) default NULL,
  `table_data` varchar(30) NOT NULL default '',
  `field` varchar(30) NOT NULL default '',
  `value` varchar(30) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 PACK_KEYS=0 AUTO_INCREMENT=75 ;

-- 
-- Dumping data for table `options`
-- 

INSERT INTO `options` VALUES (1, 0, 'ticket', 'short', 'Num');
INSERT INTO `options` VALUES (2, 0, 'ticket', 'long', 'Ticket Number');
INSERT INTO `options` VALUES (3, 1, 'ticket', 'short', 'Date');
INSERT INTO `options` VALUES (4, 1, 'ticket', 'long', 'Issue date');
INSERT INTO `options` VALUES (5, 2, 'ticket', 'short', 'Start');
INSERT INTO `options` VALUES (6, 2, 'ticket', 'long', 'Problem Starts');
INSERT INTO `options` VALUES (7, 3, 'ticket', 'short', 'End');
INSERT INTO `options` VALUES (8, 3, 'ticket', 'long', 'Ticket Ends');
INSERT INTO `options` VALUES (9, 4, 'ticket', 'short', 'Contact');
INSERT INTO `options` VALUES (10, 4, 'ticket', 'long', 'Contact Info');
INSERT INTO `options` VALUES (11, 5, 'ticket', 'short', 'Affected');
INSERT INTO `options` VALUES (12, 5, 'ticket', 'long', 'What is Affected');
INSERT INTO `options` VALUES (13, 6, 'ticket', 'short', 'Descr.');
INSERT INTO `options` VALUES (14, 6, 'ticket', 'long', 'Problem Description');
INSERT INTO `options` VALUES (15, 7, 'ticket', 'short', 'Status');
INSERT INTO `options` VALUES (16, 7, 'ticket', 'long', 'Status');
INSERT INTO `options` VALUES (17, 8, 'ticket', 'short', 'Owner');
INSERT INTO `options` VALUES (18, 8, 'ticket', 'long', 'Owner');
INSERT INTO `options` VALUES (19, 9, 'ticket', 'short', 'Severity');
INSERT INTO `options` VALUES (20, 9, 'ticket', 'long', 'Severity');
INSERT INTO `options` VALUES (21, 10, 'ticket', 'short', 'Location');
INSERT INTO `options` VALUES (22, 10, 'ticket', 'long', 'Affected Location');
INSERT INTO `options` VALUES (23, 0, 'severity', 'Normal', '0');
INSERT INTO `options` VALUES (24, 1, 'severity', 'Medium', '1');
INSERT INTO `options` VALUES (25, 2, 'severity', 'High', '2');
INSERT INTO `options` VALUES (26, 0, 'location', '--', '0');
INSERT INTO `options` VALUES (27, 1, 'location', 'Location 1', '1');
INSERT INTO `options` VALUES (28, 2, 'location', 'Location 2', '2');
INSERT INTO `options` VALUES (29, 3, 'location', 'Location 3', '3');
INSERT INTO `options` VALUES (40, 0, 'action', 'short', 'Num');
INSERT INTO `options` VALUES (41, 0, 'action', 'long', 'Action Number');
INSERT INTO `options` VALUES (42, 1, 'action', 'short', 'T. Num');
INSERT INTO `options` VALUES (43, 1, 'action', 'long', 'Ticket Number');
INSERT INTO `options` VALUES (44, 2, 'action', 'short', 'Date');
INSERT INTO `options` VALUES (45, 2, 'action', 'long', 'Action Date');
INSERT INTO `options` VALUES (46, 3, 'action', 'short', 'Desc.');
INSERT INTO `options` VALUES (47, 3, 'action', 'long', 'Description');
INSERT INTO `options` VALUES (48, 4, 'action', 'short', 'Creator');
INSERT INTO `options` VALUES (49, 4, 'action', 'long', 'Action Creator');
INSERT INTO `options` VALUES (50, 5, 'action', 'short', 'Type');
INSERT INTO `options` VALUES (51, 5, 'action', 'long', 'Action Type');
INSERT INTO `options` VALUES (52, 1, 'status', 'Open', '1');
INSERT INTO `options` VALUES (53, 2, 'status', 'Closed', '2');
INSERT INTO `options` VALUES (54, 0, 'action_description', '--', '0');
INSERT INTO `options` VALUES (55, 1, 'action_description', 'Open', '1');
INSERT INTO `options` VALUES (56, 2, 'action_description', 'Open', '2');
INSERT INTO `options` VALUES (57, 3, 'action_description', 'Close', '3');
INSERT INTO `options` VALUES (58, 4, 'action_description', 'Owner', '4');
INSERT INTO `options` VALUES (59, 5, 'action_description', 'Problem Start', '5');
INSERT INTO `options` VALUES (60, 6, 'action_description', 'Problem End', '6');
INSERT INTO `options` VALUES (61, 7, 'action_description', 'Affected', '7');
INSERT INTO `options` VALUES (62, 8, 'action_description', 'Contact', '8');
INSERT INTO `options` VALUES (63, 9, 'action_description', 'Severity', '9');
INSERT INTO `options` VALUES (64, 10, 'action_description', 'Comment', '10');
INSERT INTO `options` VALUES (65, 0, 'ticket', 'in_listing', '0');
INSERT INTO `options` VALUES (66, 1, 'ticket', 'in_listing', '1');
INSERT INTO `options` VALUES (67, 2, 'ticket', 'in_listing', '6');
INSERT INTO `options` VALUES (68, 3, 'ticket', 'in_listing', '5');
INSERT INTO `options` VALUES (69, 4, 'ticket', 'in_listing', '8');
INSERT INTO `options` VALUES (70, 5, 'ticket', 'in_listing', '9');
INSERT INTO `options` VALUES (71, 6, 'ticket', 'in_listing', '10');
INSERT INTO `options` VALUES (72, 0, 'action', 'in_listing', '2');
INSERT INTO `options` VALUES (73, 1, 'action', 'in_listing', '4');
INSERT INTO `options` VALUES (74, 2, 'action', 'in_listing', '3');

-- --------------------------------------------------------

-- 
-- Table structure for table `settings`
-- 

CREATE TABLE `settings` (
  `id` int(8) NOT NULL auto_increment,
  `name` tinytext,
  `value` tinytext,
  `test` varchar(50) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=19 ;

-- 
-- Dumping data for table `settings`
-- 

INSERT INTO `settings` VALUES (1, 'version', '0.915b', NULL);
INSERT INTO `settings` VALUES (2, 'host', 'localhost.localdomain', NULL);
INSERT INTO `settings` VALUES (3, 'framesize', '50', NULL);
INSERT INTO `settings` VALUES (4, 'frameborder', '1', NULL);
INSERT INTO `settings` VALUES (5, 'allow_notify', '0', NULL);
INSERT INTO `settings` VALUES (6, 'login_banner', '<h3>Welcome to Ticket</h3>', NULL);
INSERT INTO `settings` VALUES (7, 'allow_anonymous_login', '0', NULL);
INSERT INTO `settings` VALUES (8, 'abbreviate_description', '65', NULL);
INSERT INTO `settings` VALUES (9, 'abbreviate_affected', '30', NULL);
INSERT INTO `settings` VALUES (10, 'validate_email', '1', NULL);
INSERT INTO `settings` VALUES (11, 'allow_custom_tags', '1', NULL);
INSERT INTO `settings` VALUES (12, 'debug_value', '0', NULL);
INSERT INTO `settings` VALUES (13, 'organization', 'Your Company, Inc.', NULL);
INSERT INTO `settings` VALUES (14, 'admin_email', 'your.email@goes.here.com', NULL);
INSERT INTO `settings` VALUES (15, 'admin_phone', '555-123-4567 ext. <b>666</b> or <b>6969</b>', NULL);
INSERT INTO `settings` VALUES (16, 'admin_name', 'Your Name Goes Here', NULL);
INSERT INTO `settings` VALUES (17, 'admin_department', 'Your Department', NULL);
INSERT INTO `settings` VALUES (18, 'ticket_website', 'http://www.yourcompany.com/ticket/', NULL);

-- --------------------------------------------------------

-- 
-- Table structure for table `ticket`
-- 

CREATE TABLE `ticket` (
  `id` int(8) NOT NULL auto_increment,
  `t_date` datetime default NULL,
  `problemstart` datetime default NULL,
  `problemend` datetime default NULL,
  `scope` text NOT NULL,
  `affected` text,
  `description` text NOT NULL,
  `status` tinyint(1) NOT NULL default '1',
  `owner` tinyint(4) NOT NULL default '1',
  `severity` int(2) NOT NULL default '0',
  `location` tinyint(4) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `ID` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

-- 
-- Dumping data for table `ticket`
-- 

INSERT INTO `ticket` VALUES (1, '2003-06-20 14:43:28', '2003-06-20 02:43:00', '2003-06-20 02:43:00', 'This is a test', 'This is a test. This is a test', 'This is a test. This is a test. This is a test. This is a test. This is a test. This is a test. This is a test This is a test', 2, 2, 0, 2);

-- --------------------------------------------------------

-- 
-- Table structure for table `user`
-- 

CREATE TABLE `user` (
  `id` int(8) NOT NULL auto_increment,
  `passwd` tinytext,
  `info` text NOT NULL,
  `user` tinytext,
  `admin` tinyint(1) default NULL,
  `email` tinytext,
  `ticket_per_page` tinyint(1) default NULL,
  `sort_desc` tinyint(1) default '0',
  `sortorder` tinytext,
  `reporting` tinyint(1) default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `ID` (`id`),
  KEY `ID_2` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

-- 
-- Dumping data for table `user`
-- 

INSERT INTO `user` VALUES (1, '28eeeff35b6f235a', 'Un-assigned', 'unassigned', 0, 'your.email@your.company.com', 0, 0, 't_date', 0);
INSERT INTO `user` VALUES (2, '43e9a4ab75570f5b', 'administrator', 'admin', 2, 'your.email@your.company.com', 0, 0, 't_date', 0);
