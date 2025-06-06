-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jun 06, 2025 at 09:15 AM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `p1`
--

-- --------------------------------------------------------

--
-- Table structure for table `currvolt`
--

CREATE TABLE `currvolt` (
  `voltage` float NOT NULL,
  `soc` float NOT NULL,
  `current` float NOT NULL,
  `soh` float NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `currvolt`
--

INSERT INTO `currvolt` (`voltage`, `soc`, `current`, `soh`, `timestamp`) VALUES
(0, 0, 0, 96.875, '2025-05-29 07:13:47'),
(0, 0, 0.354831, 96.875, '2025-05-29 07:13:48'),
(5.20119, 0, 0.374544, 96.875, '2025-05-29 07:13:51'),
(5.18837, 0, 0.354831, 96.875, '2025-05-29 07:13:55'),
(5.20054, 0, 0.354831, 96.875, '2025-05-29 07:13:55'),
(5.19748, 0, 0.354831, 96.875, '2025-05-29 07:13:56'),
(5.20788, 0, 0.374544, 96.875, '2025-05-29 07:13:57'),
(5.20659, 0, 0.335118, 96.875, '2025-05-29 07:13:59'),
(5.1999, 0, 0.354831, 96.875, '2025-05-29 07:14:04'),
(5.20417, 0, 0.394257, 96.875, '2025-05-29 07:14:04'),
(5.20232, 0, 0.335118, 96.875, '2025-05-29 07:14:11'),
(5.19595, 0, 0.335118, 96.875, '2025-05-29 07:14:12'),
(5.19829, 0, 0.335118, 96.875, '2025-05-29 07:14:12'),
(5.20836, 0, 0.354831, 96.875, '2025-05-29 07:14:14'),
(0, 0, 0, 96.875, '2025-05-29 07:14:14'),
(0, 0, 0, 96.875, '2025-05-29 07:14:16'),
(0, 0, 0, 96.875, '2025-05-29 07:14:19'),
(0, 0, 0, 96.875, '2025-05-29 07:14:19'),
(0, 0, 0, 96.875, '2025-05-29 07:14:21');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
