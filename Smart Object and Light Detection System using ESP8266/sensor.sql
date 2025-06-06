-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jun 06, 2025 at 09:10 AM
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
-- Table structure for table `sensor`
--

CREATE TABLE `sensor` (
  `ir_value` int(11) DEFAULT NULL,
  `ldr_value` int(11) DEFAULT NULL,
  `timestamp` datetime DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `sensor`
--

INSERT INTO `sensor` (`ir_value`, `ldr_value`, `timestamp`) VALUES
(0, 43, '2025-05-24 13:03:45'),
(1, 43, '2025-05-24 13:05:45'),
(0, 1024, '2025-05-24 13:06:01'),
(1, 1024, '2025-05-24 13:06:04'),
(0, 1024, '2025-05-24 13:06:07'),
(0, 43, '2025-05-24 13:06:12'),
(1, 1024, '2025-05-24 13:06:44'),
(1, 1024, '2025-05-24 13:06:55'),
(1, 1024, '2025-05-24 13:07:19'),
(0, 43, '2025-05-24 13:07:22'),
(0, 43, '2025-05-24 13:07:44'),
(0, 43, '2025-05-24 13:08:31'),
(0, 43, '2025-05-24 13:08:57'),
(0, 43, '2025-05-24 13:09:23'),
(0, 43, '2025-05-24 13:09:31'),
(0, 43, '2025-05-24 13:09:34'),
(0, 43, '2025-05-24 13:11:43'),
(0, 43, '2025-05-24 13:11:54'),
(0, 43, '2025-05-24 13:12:05'),
(0, 43, '2025-05-24 13:12:34'),
(0, 1024, '2025-05-24 13:13:34'),
(1, 1024, '2025-05-24 13:13:39'),
(0, 1024, '2025-05-24 13:13:48'),
(1, 1024, '2025-05-24 13:14:19'),
(1, 1024, '2025-05-24 13:14:21'),
(1, 43, '2025-05-24 13:15:09'),
(1, 1024, '2025-05-24 13:15:22'),
(0, 1024, '2025-05-24 13:15:39'),
(0, 44, '2025-05-24 13:15:42'),
(0, 43, '2025-05-24 13:16:25'),
(0, 43, '2025-05-24 13:16:44');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
