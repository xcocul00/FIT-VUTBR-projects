

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


--
-- Databáze: `iisdb`
--

-- --------------------------------------------------------

--
-- Struktura tabulky `Host_hotela`
--

CREATE TABLE IF NOT EXISTS `Host_hotela` (
  `Host_id` int(11) NOT NULL,
  `Meno` varchar(20) NOT NULL,
  `Priezvisko` varchar(20) NOT NULL,
  `email` varchar(50) DEFAULT NULL,
  `Cislo_dokladu_totoznosti` varchar(20) NOT NULL,
  `Login` varchar(10) DEFAULT NULL,
  `Password` varchar(10) DEFAULT NULL,
  `Role` varchar(10) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `Host_hotela`
--

INSERT INTO `Host_hotela` (`Host_id`, `Meno`, `Priezvisko`, `email`, `Cislo_dokladu_totoznosti`, `Login`, `Password`, `Role`) VALUES
(1, 'Wade', 'Wilson', 'a2dv6rw5@gmail.com', '1245124', 'david', '123456', 'guest'),
(3, 'Maroš', 'Cocuľa', 'xcocul00@stud.fit.vutbr.cz', '1452452', 'Cocula', 'aaa', 'guest');

-- --------------------------------------------------------

--
-- Struktura tabulky `Izby`
--

CREATE TABLE IF NOT EXISTS `Izby` (
  `Izby_id` int(11) NOT NULL,
  `Typ_izby_id_typ` int(11) DEFAULT NULL,
  `Cislo_izby` int(11) NOT NULL,
  `Poschodie` int(11) NOT NULL,
  `Aktivna` int(11) DEFAULT '1'
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `Izby`
--

INSERT INTO `Izby` (`Izby_id`, `Typ_izby_id_typ`, `Cislo_izby`, `Poschodie`, `Aktivna`) VALUES
(1, 1, 100, 1, 1),
(2, 1, 101, 1, 1),
(3, 1, 102, 1, 1),
(4, 1, 103, 1, 1),
(5, 2, 200, 2, 1),
(6, 2, 201, 2, 1),
(7, 2, 202, 2, 1),
(8, 3, 203, 2, 1),
(9, 3, 204, 2, 1),
(10, 4, 300, 3, 1);

-- --------------------------------------------------------

--
-- Struktura tabulky `Platby`
--

CREATE TABLE IF NOT EXISTS `Platby` (
  `Platby_id` int(11) NOT NULL,
  `Faktura` varchar(30) DEFAULT NULL,
  `Host_id_host` int(11) DEFAULT NULL,
  `Zamestnanec_id_zamestnanec` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Struktura tabulky `Sluzby`
--

CREATE TABLE IF NOT EXISTS `Sluzby` (
  `Sluzby_id` int(11) NOT NULL,
  `Nazov_sluzby` varchar(30) NOT NULL,
  `Popis_sluzby` varchar(45) DEFAULT NULL,
  `Obmedzenia` varchar(45) DEFAULT 'none',
  `Cena_sluzby` float NOT NULL,
  `Aktivna` int(11) DEFAULT '1'
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `Sluzby`
--

INSERT INTO `Sluzby` (`Sluzby_id`, `Nazov_sluzby`, `Popis_sluzby`, `Obmedzenia`, `Cena_sluzby`, `Aktivna`) VALUES
(1, 'Raňajky', 'Strava - raňajky', 'V čase od 7:00 - 10:00', 3, 1),
(2, 'Obed', 'Strava - obed', 'V čase od 11:30 - 13:00', 5, 1),
(3, 'Večera', 'Strava - večera', 'V čase od 17:00 - 20:00', 3, 1),
(4, 'Domáce zviera', 'Prṕlatok za domáce zviera', '', 10, 1);

-- --------------------------------------------------------

--
-- Struktura tabulky `Typ_izby`
--

CREATE TABLE IF NOT EXISTS `Typ_izby` (
  `Typ_izby_id` int(11) NOT NULL,
  `Typ_izby` varchar(45) NOT NULL,
  `Cena_izby` float NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `Typ_izby`
--

INSERT INTO `Typ_izby` (`Typ_izby_id`, `Typ_izby`, `Cena_izby`) VALUES
(1, 'Dvojlôžková izba štandard', 30),
(2, 'Trojlôžková izba štandard', 35),
(3, 'Dvojlôžková izba lucrative', 50),
(4, 'Apartmán', 70);

-- --------------------------------------------------------

--
-- Struktura tabulky `Ubytovania`
--

CREATE TABLE IF NOT EXISTS `Ubytovania` (
  `Ubytovania_id` int(11) NOT NULL,
  `Dlzka_pobytu` varchar(20) NOT NULL,
  `Cena` float NOT NULL,
  `Typ_ubytovania` varchar(10) NOT NULL,
  `Zamestnanec_id_zamestnanec` int(11) DEFAULT NULL,
  `Host_id_host` int(11) DEFAULT NULL,
  `Vystavene_faktury_id_faktury` int(11) DEFAULT NULL,
  `date_from` varchar(10) DEFAULT NULL,
  `date_to` varchar(10) DEFAULT NULL,
  `Komentar` varchar(500) DEFAULT NULL,
  `Sezona` varchar(10) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `Ubytovania`
--

INSERT INTO `Ubytovania` (`Ubytovania_id`, `Dlzka_pobytu`, `Cena`, `Typ_ubytovania`, `Zamestnanec_id_zamestnanec`, `Host_id_host`, `Vystavene_faktury_id_faktury`, `date_from`, `date_to`, `Komentar`, `Sezona`) VALUES
(5, '6', 888, 'Ubytovanie', 1, 1, 9, '04/12/2016', '09/12/2016', 'Som hladny', 'Zima'),
(7, '8', 1264, 'Rezervacia', NULL, 3, 11, '16/02/2017', '23/02/2017', 'Domace zviera - pes', 'Zima'),
(8, '8', 304, 'Rezervacia', NULL, 1, 12, '17/02/2017', '24/02/2017', '', 'Zima'),
(9, '7', 931, 'Rezervacia', NULL, 1, 13, '16/03/2017', '22/03/2017', '', 'Zima');

-- --------------------------------------------------------

--
-- Struktura tabulky `Ubytovanie_izba`
--

CREATE TABLE IF NOT EXISTS `Ubytovanie_izba` (
  `Ubytovanie_izba_id` int(11) NOT NULL,
  `Izby_id_izby` int(11) DEFAULT NULL,
  `Ubytovanie_id_ubytovanie` int(11) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `Ubytovanie_izba`
--

INSERT INTO `Ubytovanie_izba` (`Ubytovanie_izba_id`, `Izby_id_izby`, `Ubytovanie_id_ubytovanie`) VALUES
(8, 5, 5),
(9, 6, 5),
(10, 10, 5),
(14, 1, 7),
(15, 2, 7),
(16, 3, 7),
(17, 8, 7),
(18, 5, 8),
(19, 1, 9),
(20, 2, 9),
(21, 10, 9);

-- --------------------------------------------------------

--
-- Struktura tabulky `Ubytovanie_sluzby`
--

CREATE TABLE IF NOT EXISTS `Ubytovanie_sluzby` (
  `Ubytovanie_sluzby_id` int(11) NOT NULL,
  `Sluzby_id_sluzby` int(11) DEFAULT NULL,
  `Ubytovanie_id_ubytovanie` int(11) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `Ubytovanie_sluzby`
--

INSERT INTO `Ubytovanie_sluzby` (`Ubytovanie_sluzby_id`, `Sluzby_id_sluzby`, `Ubytovanie_id_ubytovanie`) VALUES
(5, 1, 5),
(6, 2, 5),
(9, 1, 7),
(10, 2, 7),
(11, 4, 7),
(12, 1, 8),
(13, 1, 9);

-- --------------------------------------------------------

--
-- Struktura tabulky `Vystavene_faktury`
--

CREATE TABLE IF NOT EXISTS `Vystavene_faktury` (
  `Faktury_id` int(11) NOT NULL,
  `Cislo_faktury` varchar(30) NOT NULL,
  `Platby_id_platby` int(11) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `Vystavene_faktury`
--

INSERT INTO `Vystavene_faktury` (`Faktury_id`, `Cislo_faktury`, `Platby_id_platby`) VALUES
(1, 'AA1', NULL),
(2, 'AA2', NULL),
(3, 'AA3', NULL),
(4, 'AA4', NULL),
(5, 'AA5', NULL),
(6, 'AA6', NULL),
(7, 'AA7', NULL),
(8, 'AA8', NULL),
(9, 'AA9', NULL),
(10, 'AA10', NULL),
(11, 'AA11', NULL),
(12, 'AA12', NULL),
(13, 'AA13', NULL);

-- --------------------------------------------------------

--
-- Struktura tabulky `Zamestnanec`
--

CREATE TABLE IF NOT EXISTS `Zamestnanec` (
  `Zamestnanec_id` int(11) NOT NULL,
  `Meno` varchar(20) NOT NULL,
  `Priezvisko` varchar(20) NOT NULL,
  `Rodne_cislo` decimal(10,0) NOT NULL,
  `Pozicia` varchar(20) NOT NULL,
  `Obcianstvo` varchar(20) NOT NULL,
  `Login` varchar(10) DEFAULT NULL,
  `Password` varchar(10) DEFAULT NULL,
  `Role` varchar(10) DEFAULT NULL,
  `Aktivna` int(11) DEFAULT '1'
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `Zamestnanec`
--

INSERT INTO `Zamestnanec` (`Zamestnanec_id`, `Meno`, `Priezvisko`, `Rodne_cislo`, `Pozicia`, `Obcianstvo`, `Login`, `Password`, `Role`, `Aktivna`) VALUES
(1, 'David', 'Prexta', 9555554214, 'Recepčný', 'SK', 'Prexta', 'aaa', 'employee', 1),
(2, 'John', 'Doe', 1235456, 'Administrátor', 'SK', 'Admin', 'aaa', 'admin', 1);

--
-- Klíče pro exportované tabulky
--

--
-- Klíče pro tabulku `Host_hotela`
--
ALTER TABLE `Host_hotela`
  ADD PRIMARY KEY (`Host_id`);

--
-- Klíče pro tabulku `Izby`
--
ALTER TABLE `Izby`
  ADD PRIMARY KEY (`Izby_id`),
  ADD KEY `Typ_izby_id_typ` (`Typ_izby_id_typ`);

--
-- Klíče pro tabulku `Platby`
--
ALTER TABLE `Platby`
  ADD PRIMARY KEY (`Platby_id`),
  ADD KEY `Zamestnanec_id_zamestnanec` (`Zamestnanec_id_zamestnanec`),
  ADD KEY `Host_id_host` (`Host_id_host`);

--
-- Klíče pro tabulku `Sluzby`
--
ALTER TABLE `Sluzby`
  ADD PRIMARY KEY (`Sluzby_id`);

--
-- Klíče pro tabulku `Typ_izby`
--
ALTER TABLE `Typ_izby`
  ADD PRIMARY KEY (`Typ_izby_id`);

--
-- Klíče pro tabulku `Ubytovania`
--
ALTER TABLE `Ubytovania`
  ADD PRIMARY KEY (`Ubytovania_id`),
  ADD KEY `Zamestnanec_id_zamestnanec` (`Zamestnanec_id_zamestnanec`),
  ADD KEY `Host_id_host` (`Host_id_host`),
  ADD KEY `Vystavene_faktury_id_faktury` (`Vystavene_faktury_id_faktury`);

--
-- Klíče pro tabulku `Ubytovanie_izba`
--
ALTER TABLE `Ubytovanie_izba`
  ADD PRIMARY KEY (`Ubytovanie_izba_id`),
  ADD KEY `Izby_id_izby` (`Izby_id_izby`),
  ADD KEY `Ubytovanie_id_ubytovanie` (`Ubytovanie_id_ubytovanie`);

--
-- Klíče pro tabulku `Ubytovanie_sluzby`
--
ALTER TABLE `Ubytovanie_sluzby`
  ADD PRIMARY KEY (`Ubytovanie_sluzby_id`),
  ADD KEY `Sluzby_id_sluzby` (`Sluzby_id_sluzby`),
  ADD KEY `Ubytovanie_id_ubytovanie` (`Ubytovanie_id_ubytovanie`);

--
-- Klíče pro tabulku `Vystavene_faktury`
--
ALTER TABLE `Vystavene_faktury`
  ADD PRIMARY KEY (`Faktury_id`),
  ADD KEY `Platby_id_platby` (`Platby_id_platby`);

--
-- Klíče pro tabulku `Zamestnanec`
--
ALTER TABLE `Zamestnanec`
  ADD PRIMARY KEY (`Zamestnanec_id`);

--
-- AUTO_INCREMENT pro tabulky
--

--
-- AUTO_INCREMENT pro tabulku `Host_hotela`
--
ALTER TABLE `Host_hotela`
  MODIFY `Host_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=4;
--
-- AUTO_INCREMENT pro tabulku `Izby`
--
ALTER TABLE `Izby`
  MODIFY `Izby_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=11;
--
-- AUTO_INCREMENT pro tabulku `Platby`
--
ALTER TABLE `Platby`
  MODIFY `Platby_id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pro tabulku `Sluzby`
--
ALTER TABLE `Sluzby`
  MODIFY `Sluzby_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT pro tabulku `Typ_izby`
--
ALTER TABLE `Typ_izby`
  MODIFY `Typ_izby_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT pro tabulku `Ubytovania`
--
ALTER TABLE `Ubytovania`
  MODIFY `Ubytovania_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=10;
--
-- AUTO_INCREMENT pro tabulku `Ubytovanie_izba`
--
ALTER TABLE `Ubytovanie_izba`
  MODIFY `Ubytovanie_izba_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=22;
--
-- AUTO_INCREMENT pro tabulku `Ubytovanie_sluzby`
--
ALTER TABLE `Ubytovanie_sluzby`
  MODIFY `Ubytovanie_sluzby_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=14;
--
-- AUTO_INCREMENT pro tabulku `Vystavene_faktury`
--
ALTER TABLE `Vystavene_faktury`
  MODIFY `Faktury_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=14;
--
-- AUTO_INCREMENT pro tabulku `Zamestnanec`
--
ALTER TABLE `Zamestnanec`
  MODIFY `Zamestnanec_id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=3;
--
-- Omezení pro exportované tabulky
--

--
-- Omezení pro tabulku `Izby`
--
ALTER TABLE `Izby`
  ADD CONSTRAINT `Izby_ibfk_1` FOREIGN KEY (`Typ_izby_id_typ`) REFERENCES `Typ_izby` (`Typ_izby_id`);

--
-- Omezení pro tabulku `Platby`
--
ALTER TABLE `Platby`
  ADD CONSTRAINT `Platby_ibfk_1` FOREIGN KEY (`Zamestnanec_id_zamestnanec`) REFERENCES `Zamestnanec` (`Zamestnanec_id`),
  ADD CONSTRAINT `Platby_ibfk_2` FOREIGN KEY (`Host_id_host`) REFERENCES `Host_hotela` (`Host_id`);

--
-- Omezení pro tabulku `Ubytovania`
--
ALTER TABLE `Ubytovania`
  ADD CONSTRAINT `Ubytovania_ibfk_1` FOREIGN KEY (`Zamestnanec_id_zamestnanec`) REFERENCES `Zamestnanec` (`Zamestnanec_id`),
  ADD CONSTRAINT `Ubytovania_ibfk_2` FOREIGN KEY (`Host_id_host`) REFERENCES `Host_hotela` (`Host_id`),
  ADD CONSTRAINT `Ubytovania_ibfk_3` FOREIGN KEY (`Vystavene_faktury_id_faktury`) REFERENCES `Vystavene_faktury` (`Faktury_id`);

--
-- Omezení pro tabulku `Ubytovanie_izba`
--
ALTER TABLE `Ubytovanie_izba`
  ADD CONSTRAINT `Ubytovanie_izba_ibfk_1` FOREIGN KEY (`Izby_id_izby`) REFERENCES `Izby` (`Izby_id`),
  ADD CONSTRAINT `Ubytovanie_izba_ibfk_2` FOREIGN KEY (`Ubytovanie_id_ubytovanie`) REFERENCES `Ubytovania` (`Ubytovania_id`);

--
-- Omezení pro tabulku `Ubytovanie_sluzby`
--
ALTER TABLE `Ubytovanie_sluzby`
  ADD CONSTRAINT `Ubytovanie_sluzby_ibfk_1` FOREIGN KEY (`Sluzby_id_sluzby`) REFERENCES `Sluzby` (`Sluzby_id`),
  ADD CONSTRAINT `Ubytovanie_sluzby_ibfk_2` FOREIGN KEY (`Ubytovanie_id_ubytovanie`) REFERENCES `Ubytovania` (`Ubytovania_id`);

--
-- Omezení pro tabulku `Vystavene_faktury`
--
ALTER TABLE `Vystavene_faktury`
  ADD CONSTRAINT `Vystavene_faktury_ibfk_1` FOREIGN KEY (`Platby_id_platby`) REFERENCES `Platby` (`Platby_id`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
