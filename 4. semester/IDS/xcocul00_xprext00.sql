-- Projekt pre predmet IDS
-- Varianta zadania : Hotel
-- Autor : Maros Cocula (xcocul00)
--         David Prexta (xprext00)
---------------------------------------------------------------------------
-- CAST 2
---------------------------------------------------------------------------
-- vymazanie tabuliek pred zacatim skriptu

DROP TABLE Ubytovania CASCADE CONSTRAINTS;
DROP TABLE Ubytovanie_izba CASCADE CONSTRAINTS;
DROP TABLE Izby CASCADE CONSTRAINTS;
DROP TABLE Typ_izby CASCADE CONSTRAINTS;
DROP TABLE Ubytovanie_sluzby CASCADE CONSTRAINTS;
DROP TABLE Sluzby CASCADE CONSTRAINTS;
DROP TABLE Ceny_sluzieb CASCADE CONSTRAINTS;
DROP TABLE Vystavene_faktury CASCADE CONSTRAINTS;
DROP TABLE Host_hotela CASCADE CONSTRAINTS;
DROP TABLE Platby CASCADE CONSTRAINTS;
DROP TABLE Zamestnanec CASCADE CONSTRAINTS;

-- vymazanie sekvencii pred zacatim skriptu
DROP SEQUENCE TYP_IZBY_SEQ;
DROP SEQUENCE IZBY_SEQ;
DROP SEQUENCE UBYTOVANIE_IZBA_SEQ;
DROP SEQUENCE UBYTOVANIA_SEQ;
DROP SEQUENCE VYSTAVENE_FAKTURY_SEQ;
DROP SEQUENCE HOST_HOTELA_SEQ;
DROP SEQUENCE PLATBY_SEQ;
DROP SEQUENCE ZAMESTNANEC_SEQ;
DROP SEQUENCE UBYTOVANIE_SLUZBY_SEQ;
DROP SEQUENCE SLUZBY_SEQ;
DROP SEQUENCE CENY_SLUZIEB_SEQ;

-- vymazanie pohladov
DROP MATERIALIZED VIEW zamestnanec_pozicia;
DROP MATERIALIZED VIEW LOG ON zamestnanec;

-- vymazanie indexu
DROP INDEX index_dlzka_pobytu;

-- vytvorenie sekvencii pre primarne kluce
CREATE SEQUENCE TYP_IZBY_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE IZBY_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE UBYTOVANIE_IZBA_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE UBYTOVANIA_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE VYSTAVENE_FAKTURY_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE HOST_HOTELA_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE PLATBY_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE ZAMESTNANEC_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE UBYTOVANIE_SLUZBY_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE SLUZBY_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;
CREATE SEQUENCE CENY_SLUZIEB_SEQ INCREMENT BY 1 START WITH 1 NOMAXVALUE MINVALUE 1 NOCYCLE;

-- vytvorenie jednotlivych tabuliek
CREATE TABLE Ubytovania (
Ubytovania_id  INTEGER,
Dlzka_pobytu VARCHAR(20) NOT NULL,
Cena VARCHAR(10) NOT NULL,
Sezona VARCHAR(10),
Typ_ubytovania VARCHAR(10) NOT NULL,
Zamestnanec_id_zamestnanec INTEGER,
Host_id_host INTEGER,
Vystavene_faktury_id_faktury INTEGER
); 
ALTER TABLE Ubytovania ADD CONSTRAINT PK_Ubytovanie PRIMARY KEY (Ubytovania_id);

CREATE TABLE Ubytovanie_izba (
Ubytovanie_izba_id  INTEGER,
Izby_id_izby INTEGER,
Ubytovanie_id_ubytovanie INTEGER
); 
ALTER TABLE Ubytovanie_izba ADD CONSTRAINT PK_Ubytovanie_izba PRIMARY KEY (Ubytovanie_izba_id);

CREATE TABLE Izby (
Izby_id  INTEGER,
Typ_izby_id_typ INTEGER,
Cislo_izby INT NOT NULL,
Poschodie INT NOT NULL
); 
ALTER TABLE Izby ADD CONSTRAINT PK_Izby PRIMARY KEY (Izby_id);

CREATE TABLE Typ_izby (
Typ_izby_id  INTEGER,
Typ_izby VARCHAR(45) NOT NULL,
Cena_izby VARCHAR(10) NOT NULL
); 
ALTER TABLE Typ_izby ADD CONSTRAINT PK_Typ_Izby PRIMARY KEY (Typ_izby_id);

CREATE TABLE Ubytovanie_sluzby (
Ubytovanie_sluzby_id  INTEGER,
Sluzby_id_sluzby INTEGER,
Ubytovanie_id_ubytovanie INTEGER
); 
ALTER TABLE Ubytovanie_sluzby ADD CONSTRAINT PK_Ubytovanie_sluzby PRIMARY KEY (Ubytovanie_sluzby_id);

CREATE TABLE Sluzby (
Sluzby_id  INTEGER,
Ceny_sluzieb_id_ceny INTEGER,
Nazov_sluzby VARCHAR(30) NOT NULL,
Popis_sluzby VARCHAR(45),
Obmedzenia VARCHAR(45) DEFAULT 'none'
); 
ALTER TABLE Sluzby ADD CONSTRAINT PK_Sluzby PRIMARY KEY (Sluzby_id);

CREATE TABLE Ceny_sluzieb (
Ceny_sluzieb_id  INTEGER,
Nazov_sluzby VARCHAR(30) NOT NULL,
Vyska_ceny VARCHAR(8) NOT NULL
); 
ALTER TABLE Ceny_sluzieb ADD CONSTRAINT PK_Ceny_sluzieb PRIMARY KEY (Ceny_sluzieb_id);

CREATE TABLE Vystavene_faktury (
Faktury_id  INTEGER,
Cislo_faktury VARCHAR(30) NOT NULL,
Adresat VARCHAR(20),
Cena_faktury VARCHAR(10),
Platby_id_platby INTEGER
); 
ALTER TABLE Vystavene_faktury ADD CONSTRAINT PK_Vystavene_faktury PRIMARY KEY (Faktury_id);

-- host je keyword neda sa pouzit preto Host_hotela
CREATE TABLE Host_hotela(
Host_id  INTEGER,
Meno VARCHAR(20) NOT NULL,
Priezvisko VARCHAR(20) NOT NULL,
Rodne_cislo VARCHAR(11) CHECK (Rodne_cislo LIKE '______/____' ),
Cislo_dokladu_totoznosti VARCHAR(20) NOT NULL
); 
ALTER TABLE Host_hotela ADD CONSTRAINT PK_Host_hotela PRIMARY KEY (Host_id);

CREATE TABLE Platby(
Platby_id  INTEGER,
Faktura VARCHAR(30),
Host_id_host  INTEGER,
Zamestnanec_id_zamestnanec  INTEGER
); 
ALTER TABLE Platby ADD CONSTRAINT PK_Platby PRIMARY KEY (Platby_id);

CREATE TABLE Zamestnanec(
Zamestnanec_id  INTEGER,
Meno VARCHAR(20) NOT NULL,
Priezvisko VARCHAR(20) NOT NULL,
Rodne_cislo NUMBER (10) NOT NULL,
Pozicia VARCHAR(20) NOT NULL,
Obcianstvo VARCHAR(20) NOT NULL
); 
ALTER TABLE Zamestnanec ADD CONSTRAINT PK_Zamestnanec PRIMARY KEY (Zamestnanec_id);

---------------------Foreign key definition--------------------------------
--add foregin keys to table Ubytovania
ALTER TABLE Ubytovania ADD CONSTRAINT FK_Zamestnanec_id_ub FOREIGN KEY (Zamestnanec_id_zamestnanec) REFERENCES Zamestnanec (Zamestnanec_id);
ALTER TABLE Ubytovania ADD CONSTRAINT FK_Host_id_ub FOREIGN KEY (Host_id_host) REFERENCES Host_hotela (Host_id);
ALTER TABLE Ubytovania ADD CONSTRAINT FK_Faktury_id_ub FOREIGN KEY (Vystavene_faktury_id_faktury) REFERENCES Vystavene_faktury (Faktury_id);

--add foregin keys to table Platby
ALTER TABLE Platby ADD CONSTRAINT FK_Zamestnanec_id_pl FOREIGN KEY (Zamestnanec_id_zamestnanec) REFERENCES Zamestnanec (Zamestnanec_id);
ALTER TABLE Platby ADD CONSTRAINT FK_Host_id_pl FOREIGN KEY (Host_id_host) REFERENCES Host_hotela (Host_id);

--add foregin keys to table Vystavene_faktury
ALTER TABLE Vystavene_faktury ADD CONSTRAINT FK_platby_id_fa FOREIGN KEY (Platby_id_platby) REFERENCES Platby (Platby_id);

--add foregin keys to table Izby
ALTER TABLE Izby ADD CONSTRAINT FK_typ_izby_id_iz FOREIGN KEY (Typ_izby_id_typ) REFERENCES Typ_izby (Typ_izby_id);

--add foregin keys to table Ubytovanie_izba
ALTER TABLE Ubytovanie_izba ADD CONSTRAINT FK_id_izby_ui FOREIGN KEY (Izby_id_izby) REFERENCES Izby (Izby_id);
ALTER TABLE Ubytovanie_izba ADD CONSTRAINT FK_id_ubytovanie_ui FOREIGN KEY (Ubytovanie_id_ubytovanie) REFERENCES Ubytovania (Ubytovania_id);

--add foregin keys to table Ubytovanie_sluzby
ALTER TABLE Ubytovanie_sluzby ADD CONSTRAINT FK_id_sluzby_us FOREIGN KEY (Sluzby_id_sluzby) REFERENCES Sluzby (Sluzby_id);
ALTER TABLE Ubytovanie_sluzby ADD CONSTRAINT FK_id_ubytovanie_us FOREIGN KEY (Ubytovanie_id_ubytovanie) REFERENCES Ubytovania (Ubytovania_id);

--add foregin keys to table Sluzby
ALTER TABLE Sluzby ADD CONSTRAINT FK_cena_sluzby FOREIGN KEY (Ceny_sluzieb_id_ceny) REFERENCES Ceny_sluzieb (Ceny_sluzieb_id);

-------------------End of foregin key definition----------------------------

 
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Paul', 'Jones', '1922402272', 'Kuchar/ka', 'CZ');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Steven', 'Williams', '7198662152', 'recepcny/a', 'CZ');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'James', 'Simpson', '2800526610', 'Casnik/cka', 'CZ');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Virginia', 'Richardson', '4553678597', 'Upratovac/ka', 'CZ');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Larry', 'Wells', '9301102592', 'Casnik/cka', 'SK');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Doris', 'Fisher', '6955267553', 'Casnik/cka', 'SK');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Doris', 'Garcia', '3549745908', 'Casnik/cka', 'SK');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Jeffrey', 'Palmer', '7658666549', 'Casnik/cka', 'CZ');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Susan', 'Moreno', '7144042440', 'recepcny/a', 'SK');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Elizabeth', 'Jordan', '8515046500', 'Manazer', 'SK');
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Arpad', 'Wayne', '8515045200', 'Manazer', 'SK');

insert into Ceny_sluzieb (Ceny_sluzieb_id, Nazov_sluzby, Vyska_ceny) values (CENY_SLUZIEB_SEQ.NEXTVAL, 'Wellness', '€30,00');
insert into Ceny_sluzieb (Ceny_sluzieb_id, Nazov_sluzby, Vyska_ceny) values (CENY_SLUZIEB_SEQ.NEXTVAL, 'Ranajky', '€4,35');
insert into Ceny_sluzieb (Ceny_sluzieb_id, Nazov_sluzby, Vyska_ceny) values (CENY_SLUZIEB_SEQ.NEXTVAL, 'Obed', '€4,71');
insert into Ceny_sluzieb (Ceny_sluzieb_id, Nazov_sluzby, Vyska_ceny) values (CENY_SLUZIEB_SEQ.NEXTVAL, 'Vecera', '€3,65');
insert into Ceny_sluzieb (Ceny_sluzieb_id, Nazov_sluzby, Vyska_ceny) values (CENY_SLUZIEB_SEQ.NEXTVAL, 'Bazen', '€2,00');
insert into Ceny_sluzieb (Ceny_sluzieb_id, Nazov_sluzby, Vyska_ceny) values (CENY_SLUZIEB_SEQ.NEXTVAL, 'Bowling', '€6,00');

insert into Sluzby (Sluzby_id, Ceny_sluzieb_id_ceny, Nazov_sluzby, Popis_sluzby, Obmedzenia) values (SLUZBY_SEQ.NEXTVAL, 1,'Wellness','Welness sauny,masaze,kupele','masaze len v case 8:00 - 15:00');
insert into Sluzby (Sluzby_id, Ceny_sluzieb_id_ceny, Nazov_sluzby, Popis_sluzby) values (SLUZBY_SEQ.NEXTVAL, 2,'Ranajky','Strava - ranajky');
insert into Sluzby (Sluzby_id, Ceny_sluzieb_id_ceny, Nazov_sluzby, Popis_sluzby) values (SLUZBY_SEQ.NEXTVAL, 3,'Obed','Strava - obed');
insert into Sluzby (Sluzby_id, Ceny_sluzieb_id_ceny, Nazov_sluzby, Popis_sluzby) values (SLUZBY_SEQ.NEXTVAL, 4,'Vecera','Strava - vecera');
insert into Sluzby (Sluzby_id, Ceny_sluzieb_id_ceny, Nazov_sluzby, Popis_sluzby, Obmedzenia) values (SLUZBY_SEQ.NEXTVAL, 5,'Bazen','Plavanie v bazene hotela','len v case od 8:00 - 18:00');
insert into Sluzby (Sluzby_id, Ceny_sluzieb_id_ceny, Nazov_sluzby, Popis_sluzby, Obmedzenia) values (SLUZBY_SEQ.NEXTVAL, 6,'Bowling','Hra bowling','V nedelu od 15:00-19:00 zatvorene koli udrzbe');

insert into Typ_izby (Typ_izby_id, Typ_izby, Cena_izby) values (TYP_IZBY_SEQ.NEXTVAL, 'dvoj-lozkova izba standard', '€30,00');
insert into Typ_izby (Typ_izby_id, Typ_izby, Cena_izby) values (TYP_IZBY_SEQ.NEXTVAL, 'troj-lozkova izba standard', '€35,00');
insert into Typ_izby (Typ_izby_id, Typ_izby, Cena_izby) values (TYP_IZBY_SEQ.NEXTVAL, 'dvoj-lozkova izba lucrative', '€50,00');
insert into Typ_izby (Typ_izby_id, Typ_izby, Cena_izby) values (TYP_IZBY_SEQ.NEXTVAL, 'Apartman', '€70,00');

insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 1, '100', '1');
insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 1, '101', '1');
insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 1, '102', '1');
insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 1, '103', '1');
insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 2, '200', '2');
insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 2, '201', '2');
insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 2, '202', '2');
insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 3, '203', '2');
insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 3, '204', '2');
insert into Izby (Izby_id, Typ_izby_id_typ, Cislo_izby, Poschodie) values (IZBY_SEQ.NEXTVAL, 4, '300', '3');

---novy host a ubytovanie----
insert into Host_hotela (Host_id, Meno, Priezvisko, Rodne_cislo, Cislo_dokladu_totoznosti) values (HOST_HOTELA_SEQ.NEXTVAL, 'Charles', 'Long', '145241/5478', '3016157');
insert into Ubytovania (Ubytovania_id, Dlzka_pobytu, Cena, Sezona, Typ_ubytovania, Zamestnanec_id_zamestnanec, Host_id_host, Vystavene_faktury_id_faktury) values (UBYTOVANIA_SEQ.NEXTVAL, 8, '€568,14', 'Jar', 'Ubytovanie', 2, 1, null);
insert into Ubytovanie_izba (Ubytovanie_izba_id, Izby_id_izby, Ubytovanie_id_ubytovanie) values (UBYTOVANIE_IZBA_SEQ.NEXTVAL, 1, UBYTOVANIA_SEQ.CURRVAL);
insert into Ubytovanie_izba (Ubytovanie_izba_id, Izby_id_izby, Ubytovanie_id_ubytovanie) values (UBYTOVANIE_IZBA_SEQ.NEXTVAL, 2, UBYTOVANIA_SEQ.CURRVAL);
insert into Ubytovanie_sluzby (Ubytovanie_sluzby_id, Sluzby_id_sluzby, Ubytovanie_id_ubytovanie) values (UBYTOVANIE_SLUZBY_SEQ.NEXTVAL, 2, UBYTOVANIA_SEQ.CURRVAL);
insert into Ubytovanie_sluzby (Ubytovanie_sluzby_id, Sluzby_id_sluzby, Ubytovanie_id_ubytovanie) values (UBYTOVANIE_SLUZBY_SEQ.NEXTVAL, 3, UBYTOVANIA_SEQ.CURRVAL);
insert into Ubytovanie_sluzby (Ubytovanie_sluzby_id, Sluzby_id_sluzby, Ubytovanie_id_ubytovanie) values (UBYTOVANIE_SLUZBY_SEQ.NEXTVAL, 5, UBYTOVANIA_SEQ.CURRVAL);
insert into Vystavene_faktury (Faktury_id, Cislo_faktury, Adresat, Cena_faktury, Platby_id_platby) values (VYSTAVENE_FAKTURY_SEQ.NEXTVAL, 'AA4575', 'Cahrels Long', '€568,48',null);
update Ubytovania set Vystavene_faktury_id_faktury=1 where Ubytovania_id=1 ;

----nova rezervacia pre uz existujuceho hosta
insert into Ubytovania (Ubytovania_id, Dlzka_pobytu, Cena, Sezona, Typ_ubytovania, Zamestnanec_id_zamestnanec, Host_id_host, Vystavene_faktury_id_faktury) values (UBYTOVANIA_SEQ.NEXTVAL, 10, '€343,5', 'Leto', 'Rezervacia', 2, 1, null);
insert into Ubytovanie_izba (Ubytovanie_izba_id, Izby_id_izby, Ubytovanie_id_ubytovanie) values (UBYTOVANIE_IZBA_SEQ.NEXTVAL, 1, UBYTOVANIA_SEQ.CURRVAL);
insert into Ubytovanie_sluzby (Ubytovanie_sluzby_id, Sluzby_id_sluzby, Ubytovanie_id_ubytovanie) values (UBYTOVANIE_SLUZBY_SEQ.NEXTVAL, 2, UBYTOVANIA_SEQ.CURRVAL);
insert into Vystavene_faktury (Faktury_id, Cislo_faktury, Adresat, Cena_faktury, Platby_id_platby) values (VYSTAVENE_FAKTURY_SEQ.NEXTVAL, 'AB4575', 'Cahrels Long', '€343,5',null);
update Ubytovania set Vystavene_faktury_id_faktury=2 where Ubytovania_id=2;

--novy host a ubytovanie
insert into Host_hotela (Host_id, Meno, Priezvisko, Rodne_cislo, Cislo_dokladu_totoznosti) values (HOST_HOTELA_SEQ.NEXTVAL, 'Billy', 'Medina', '149852/4735', '3279912');
insert into Ubytovania (Ubytovania_id, Dlzka_pobytu, Cena, Sezona, Typ_ubytovania, Zamestnanec_id_zamestnanec, Host_id_host, Vystavene_faktury_id_faktury) values (UBYTOVANIA_SEQ.NEXTVAL, 5, '€350', 'Jar', 'Ubytovanie', 10, 2, null);
insert into Ubytovanie_izba (Ubytovanie_izba_id, Izby_id_izby, Ubytovanie_id_ubytovanie) values (UBYTOVANIE_IZBA_SEQ.NEXTVAL, 4, UBYTOVANIA_SEQ.CURRVAL);
insert into Vystavene_faktury (Faktury_id, Cislo_faktury, Adresat, Cena_faktury, Platby_id_platby) values (VYSTAVENE_FAKTURY_SEQ.NEXTVAL, 'AB9575', 'Billy Medina', '€350',null);
update Ubytovania set Vystavene_faktury_id_faktury=3 where Ubytovania_id=3;

--novy host a rezervacia
insert into Host_hotela (Host_id, Meno, Priezvisko, Rodne_cislo, Cislo_dokladu_totoznosti) values (HOST_HOTELA_SEQ.NEXTVAL, 'Christine', 'Taylor', '675182/4758', '9302816');
insert into Ubytovania (Ubytovania_id, Dlzka_pobytu, Cena, Sezona, Typ_ubytovania, Zamestnanec_id_zamestnanec, Host_id_host, Vystavene_faktury_id_faktury) values (UBYTOVANIA_SEQ.NEXTVAL, 15, '€750', 'Zima', 'Rezervacia', 11, 3, null);
insert into Ubytovanie_izba (Ubytovanie_izba_id, Izby_id_izby, Ubytovanie_id_ubytovanie) values (UBYTOVANIE_IZBA_SEQ.NEXTVAL, 3, UBYTOVANIA_SEQ.CURRVAL);

--host 1 zaplatil za vystavenu fakturu
insert into Platby (Platby_id, Faktura, Host_id_host, Zamestnanec_id_zamestnanec) values (PLATBY_SEQ.NEXTVAL, 'AA4575', 1, 9);
update Vystavene_faktury set Platby_id_platby=1 where Faktury_id=1;


---------------------------------------------------------------------------
-- CAST 3
---------------------------------------------------------------------------
-- doplnenie tabuliek pre demonstraciu selectov 
insert into Ubytovania (Ubytovania_id, Dlzka_pobytu, Cena, Sezona, Typ_ubytovania, Zamestnanec_id_zamestnanec, Host_id_host, Vystavene_faktury_id_faktury) values (UBYTOVANIA_SEQ.NEXTVAL, 4, '€225,5', 'Jar', 'Ubytovanie', 2, 1, null);

insert into Platby (Platby_id, Faktura, Host_id_host, Zamestnanec_id_zamestnanec) values (PLATBY_SEQ.NEXTVAL, 'AB4575', 2, 5);
update Vystavene_faktury set Platby_id_platby=2 where Faktury_id=2;

insert into Platby (Platby_id, Faktura, Host_id_host, Zamestnanec_id_zamestnanec) values (PLATBY_SEQ.NEXTVAL, 'AB9575', 3, 10);
update Vystavene_faktury set Platby_id_platby=3 where Faktury_id=3;
COMMIT;
--show errors;

-- spojenie dvoch tabuliek
-- vypise vsetkych hostov ktory boli ubytovani 
SELECT MENO, PRIEZVISKO, DLZKA_POBYTU, CENA 
FROM UBYTOVANIA, HOST_HOTELA
WHERE (UBYTOVANIA.Host_id_host = HOST_HOTELA.Host_id) AND (UBYTOVANIA.Typ_ubytovania= 'Ubytovanie');

-- vypise informacie o vsetkych izbach na 2 poschodi
SELECT CISLO_IZBY,TYP_IZBY,CENA_IZBY
FROM IZBY,TYP_IZBY
WHERE IZBY.Typ_izby_id_typ = TYP_IZBY.Typ_izby_id AND Poschodie = '2'
ORDER BY CISLO_IZBY;

-- spojenie troch tabuliek
-- vypise ktore platby prijal aky zamestnanec (len zaplatene ubytovania)
SELECT MENO, PRIEZVISKO, DLZKA_POBYTU, CISLO_FAKTURY, ADRESAT
FROM ZAMESTNANEC,UBYTOVANIA, VYSTAVENE_FAKTURY
WHERE (UBYTOVANIA.Zamestnanec_id_zamestnanec = ZAMESTNANEC.Zamestnanec_id) 
  AND (UBYTOVANIA.Vystavene_faktury_id_faktury= VYSTAVENE_FAKTURY.Faktury_id)
  AND (VYSTAVENE_FAKTURY.Platby_id_platby > 0);

-- predikat IN s vnorenym SELECTOM
-- vypise hosta , cislo izby a nazov sluzby ktoru si objednal Charles Long
SELECT MENO, PRIEZVISKO,CISLO_IZBY, NAZOV_SLUZBY
FROM UBYTOVANIA
INNER JOIN SLUZBY ON (UBYTOVANIA.UBYTOVANIA_ID = SLUZBY.SLUZBY_ID)
INNER JOIN HOST_HOTELA ON (UBYTOVANIA.HOST_ID_HOST = HOST_HOTELA.HOST_ID)
INNER JOIN IZBY ON (UBYTOVANIA.UBYTOVANIA_ID = IZBY.Izby_id)
AND MENO IN (SELECT MENO
              FROM HOST_HOTELA
              WHERE (HOST_HOTELA.MENO = 'Charles')
              AND (HOST_HOTELA.PRIEZVISKO = 'Long'));

-- predikat EXIST s vnorenym SELECTOM
--vypise osoby, ktore mali vytvorene rezervacie ale nedostavili sa na ubytovanie (Na tychto hosti treba davat vacsi pozor)
--vyhladavaju sa hostia z konkretnej sezony
SELECT H.* 
FROM HOST_HOTELA H, UBYTOVANIA U 
WHERE H.HOST_ID=U.HOST_ID_HOST 
  AND U.TYP_UBYTOVANIA='Rezervacia' 
  AND U.SEZONA='Zima' 
  AND NOT EXISTS 
  (SELECT * 
   FROM UBYTOVANIA U1 
   WHERE H.HOST_ID=U1.HOST_ID_HOST 
   AND U1.TYP_UBYTOVANIA='Ubytovanie');

--predikat GROUP BY s agregacnou funkciou COUNT
--vypise hosta a sezonu , pocast ktorej mal dany host viac ako 1 ubytovanie
SELECT HOST_ID,MENO,PRIEZVISKO,SEZONA 
FROM HOST_HOTELA JOIN UBYTOVANIA U ON HOST_ID=U.HOST_ID_HOST 
WHERE U.TYP_UBYTOVANIA='Ubytovanie' 
GROUP BY MENO,PRIEZVISKO,HOST_ID,SEZONA HAVING COUNT(*)>1;

-- vypise zamestnanca ktory registroval ubytovanie a zaroven zan aj prijal platbu 
SELECT U.UBYTOVANIA_ID, Z.* 
FROM ZAMESTNANEC Z, UBYTOVANIA U 
WHERE Z.ZAMESTNANEC_ID=U.ZAMESTNANEC_ID_ZAMESTNANEC 
AND (SELECT COUNT(*) 
      FROM VYSTAVENE_FAKTURY JOIN PLATBY ON PLATBY_ID_PLATBY=PLATBY_ID 
      WHERE ZAMESTNANEC_ID_ZAMESTNANEC=Z.ZAMESTNANEC_ID 
      AND FAKTURY_ID=U.VYSTAVENE_FAKTURY_ID_FAKTURY )<>0;

-- predikat GROUB BY a agregacna funkcia MAX
--vypise kazdeho hosta a k nemu najdlhsiu dobu pocas ktorej bol ubytovani a najvyssiu cenu ubytovania
SELECT HOST_ID, MENO,PRIEZVISKO, MAX(DLZKA_POBYTU) AS Najdlhsi_pobyt, MAX(CENA) AS CENA 
FROM HOST_HOTELA H JOIN UBYTOVANIA U ON HOST_ID=HOST_ID_HOST 
GROUP BY HOST_ID, MENO, PRIEZVISKO ;

-- predikat GROUP BY a funkcia MIN + ukazka vypisania prveho riadku
-- vypise najlacnejsiu izbu na 2 poschodi
-- ak maju 2 izby rovnaku cenu vyberie sa prva podla cisla izby
SELECT TYP_IZBY, CISLO_IZBY, MIN(CENA_IZBY) AS Cena_najlacnejsej_izby
FROM (
  SELECT Cena_izby, Cislo_izby, Typ_izby
  FROM IZBY
  INNER JOIN TYP_IZBY ON (IZBY.TYP_IZBY_ID_TYP = TYP_IZBY.TYP_IZBY_ID)
  WHERE POSCHODIE = '2'
  )
  WHERE ROWNUM = 1
  GROUP BY CISLO_IZBY, TYP_IZBY
  ORDER BY  Cena_najlacnejsej_izby;


---------------------------------------------------------------------------
-- CAST 4,5
---------------------------------------------------------------------------

-- trigger pre autoinkrementaciu id v tabulke Host_hotela

CREATE OR REPLACE TRIGGER autoinkrementacia
  BEFORE INSERT ON HOST_HOTELA
  FOR EACH ROW
BEGIN
  :new.HOST_ID := HOST_HOTELA_SEQ.nextval; 
END autoinkrementacia;
/
-- vlozenie do tabulky bez zadania id
insert into Host_hotela (Meno, Priezvisko, Rodne_cislo, Cislo_dokladu_totoznosti) values ('Martin', 'Sopoliga', '146289/5571', '3083107');
select * from Host_hotela;

-- trigger pre overenie rodneho cisla 
CREATE OR REPLACE TRIGGER rodne_cislo
  BEFORE INSERT OR UPDATE OF Rodne_cislo ON Zamestnanec
  FOR EACH ROW
  DECLARE
      cele_cislo Zamestnanec.Rodne_cislo%TYPE;
      den NUMBER(2);
      mesiac NUMBER(2);
      rok NUMBER(2);
      datum DATE;
  BEGIN
     cele_cislo := :NEW.Rodne_cislo;
     
     IF ( MOD(cele_cislo, 11) <> 0 ) THEN
       Raise_Application_Error (-20999, 'Neplatny format rodneho cisla (nedelitelne 11)');
     END IF;
     
     rok   := cele_cislo / 100000000;
     mesiac := MOD( (cele_cislo / 1000000), 100); 
     den   := MOD( (cele_cislo / 10000), 100);  
   
     IF ( mesiac > 50 ) THEN
      mesiac := mesiac - 50;
     END IF;
     BEGIN
        datum := den||'.'||mesiac||'.'||rok;
        EXCEPTION
        WHEN OTHERS THEN
   	    Raise_Application_Error (-20999, 'Neplatny format rodneho cisla (datum neexistuje)');
     END;
   END rodne_cislo;
/
-- vlozenie do tabulky Zamestnanec s korektnym rodnym cislom
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Peter', 'Mren', '8404211101', 'recepcny/a', 'SK');
-- vlozenie do tabulky Zamestnanec s nekorektnym rodnym cislom 999999/9999
--insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Eva', 'Moyes', '9999999999', 'recepcny/a', 'SK');   
   
---------------------------------------------------------------------------------
-- materializovany pohlad
---------------------------------------------------------------------------------
CREATE MATERIALIZED VIEW LOG ON Zamestnanec WITH PRIMARY KEY,ROWID(Pozicia)INCLUDING NEW VALUES;

CREATE MATERIALIZED VIEW zamestnanec_pozicia 
CACHE 
BUILD IMMEDIATE               
REFRESH FAST ON COMMIT                     
ENABLE QUERY REWRITE                      
AS SELECT Pozicia, count(Pozicia) as Pocet_zamestnancov
    FROM Zamestnanec 
    GROUP BY Pozicia
    ORDER BY count(Pozicia) DESC;

-- ukazka materializovaneho pohladu, aktualizuje sa az po commite
insert into Zamestnanec (Zamestnanec_id, Meno, Priezvisko, Rodne_cislo, Pozicia, Obcianstvo) values (ZAMESTNANEC_SEQ.NEXTVAL, 'Jozef', 'Cvikla', '9104270802', 'Hotelier', 'SK');
select * from zamestnanec_pozicia;
commit;
--select * from zamestnanec_pozicia;
---------------------------------------------------------------------------------
-- EXPLAIN PLAN
---------------------------------------------------------------------------------
-- bez pouzitia indexu
EXPLAIN PLAN FOR
SELECT MENO, MAX(DLZKA_POBYTU) AS Najdlhsi_pobyt 
FROM HOST_HOTELA NATURAL JOIN UBYTOVANIA 
WHERE HOST_ID=HOST_ID_HOST 
GROUP BY MENO;
SELECT * FROM TABLE(DBMS_XPLAN.display);

CREATE INDEX index_dlzka_pobytu ON UBYTOVANIA (DLZKA_POBYTU);

-- s indexom
EXPLAIN PLAN FOR
SELECT /*+ INDEX(UBYTOVANIA index_dlzka_pobytu)*/ MENO, MAX(DLZKA_POBYTU) AS Najdlhsi_pobyt 
FROM HOST_HOTELA NATURAL JOIN UBYTOVANIA 
WHERE HOST_ID=HOST_ID_HOST 
GROUP BY MENO;
SELECT * FROM TABLE(DBMS_XPLAN.display);
---------------------------------------------------------------------------------
-- procedury
---------------------------------------------------------------------------------
-- vypise percento zamestnancov ktory vykonavaju zadanu funkciu
-- priklad pouzitia exec percento_povolania('hotelier');
SET serveroutput ON;
CREATE OR REPLACE PROCEDURE percento_povolania(povolanie IN VARCHAR2)
is
  cursor obsah is select * from Zamestnanec;
  prvok obsah%ROWTYPE;
  percento NUMBER(10,2);
  pocet_zamestnancov NUMBER;
  hladane_povolanie NUMBER;
BEGIN
  pocet_zamestnancov := 0;
  hladane_povolanie := 0;
  percento := 0;
  open obsah;
  loop
    fetch obsah into prvok;
    exit when obsah%NOTFOUND;
    IF (prvok.Pozicia = povolanie) THEN
        hladane_povolanie := hladane_povolanie + 1;
    END IF;
    pocet_zamestnancov := pocet_zamestnancov + 1;
    
  end loop;
  percento := (hladane_povolanie * 100)/pocet_zamestnancov;
  IF (percento = 0) THEN 
    dbms_output.put_line('Nikoho s povolanim ' || povolanie || ' v hoteli nezamestnavame');
  ELSE
  dbms_output.put_line('Povolanie ' || povolanie || ' ma percentualne zastupenie medzi povolaniami v hoteli: '|| percento || '%');
  END IF;
EXCEPTION
  WHEN ZERO_DIVIDE THEN
    dbms_output.put_line('V hoteli sa nenachadzaju zamestnanci, delenie nulou');
  WHEN OTHERS THEN 
    Raise_Application_Error (-20206, 'Nieco sa pokazilo');
END;
/

-- ukazka procedury
exec percento_povolania('Manazer');
exec percento_povolania('Taxikar');
---------------------------------------------------------------------------------

SET serveroutput ON;
CREATE OR REPLACE PROCEDURE zaznam_ubytovani(zakaznik IN INT)
is
  cursor zaznam is select Host_id,meno,priezvisko,rodne_cislo,cena,dlzka_pobytu,typ_ubytovania 
                   FROM Host_hotela NATURAL JOIN Ubytovania 
                   WHERE Host_hotela.HOST_ID=Ubytovania.HOST_ID_HOST ;
   hladany_host zaznam%ROWTYPE;
   flag BOOLEAN;
BEGIN
  open zaznam;
  dbms_output.put_line('Zaznamy hladaneho zakaznika s id ' || zakaznik || ' :');
  flag := false;
  loop
    fetch zaznam into hladany_host;
    exit when zaznam%NOTFOUND;
    IF (hladany_host.Host_id = zakaznik ) THEN 
        dbms_output.put_line(hladany_host.meno || ' ' || hladany_host.priezvisko || ' ' || hladany_host.rodne_cislo || ' ' || hladany_host.cena );
        flag := true;
    END IF;
  end loop;
  IF (flag = false) THEN
    dbms_output.put_line('Host u nas doposial nebol ubytovany');
  END IF;
EXCEPTION
  WHEN NO_DATA_FOUND THEN
    dbms_output.put_line('Tabulka neobsahuje zaznamy');
  WHEN OTHERS THEN
    Raise_Application_Error (-20206, 'Nieco sa pokazilo');
END;
/
exec zaznam_ubytovani(1);
exec zaznam_ubytovani(100);
---------------------------------------------------------------------------------
-- pridelenie prav pre xprext00
---------------------------------------------------------------------------------
-- tabulky
GRANT ALL ON Ubytovania TO xprext00;
GRANT ALL ON Vystavene_faktury TO xprext00;
GRANT ALL ON Host_hotela TO xprext00;
GRANT ALL ON Platby TO xprext00;
GRANT READ ON Ubytovanie_izba TO xprext00;
GRANT READ ON Izby TO xprext00;
GRANT READ ON Typ_izby TO xprext00;
GRANT READ ON Ubytovanie_sluzby TO xprext00;
GRANT READ ON Sluzby TO xprext00;
GRANT READ ON Ceny_sluzieb TO xprext00;
-- sekvencie

GRANT ALL ON UBYTOVANIA_SEQ TO xprext00;
GRANT ALL ON VYSTAVENE_FAKTURY_SEQ TO xprext00;
GRANT ALL ON HOST_HOTELA_SEQ TO xprext00;
GRANT ALL ON PLATBY_SEQ TO xprext00;
-- materializovany pohlad
GRANT ALL ON zamestnanec_pozicia TO xprext00;
-- procedury
GRANT EXECUTE ON zaznam_ubytovani TO xprext00;

--show errors;
------------------------------END-----------------------------------