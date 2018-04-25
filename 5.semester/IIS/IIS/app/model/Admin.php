<?php

namespace App\Model;

use Nette;
use Nette\Security as NS;

class Admin{

	use Nette\SmartObject;
	
    private $database;

    function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}
//*************************************************************//
//***************** SERVICE ***********************************// 
//*************************************************************// 
	public function getServices(){

		$row = $this->database->table('Sluzby')->where('Aktivna', 1);

		return $row;			
	}

	public function getServiceInfo($name){

		$row=$this->database->table('Sluzby')->where('Nazov_sluzby', $name);

		return $row;			
	}

	public function AddService($values){
		$row=$this->database->table('Sluzby')->where('Nazov_sluzby', $values->name)->fetch();
		if(!$row){
		$this->database->table("Sluzby")->insert(array(
			'Nazov_sluzby' => $values->name,
			'Popis_sluzby' => $values->desc,
			'Obmedzenia' => $values->rest,
			'Cena_sluzby' => $values->cost,
		));
	}
	}

	public function ChangeService($values, $name){

		$this->database->table("Sluzby")->where('Nazov_sluzby',$name)->update(array(
			'Nazov_sluzby' => $values->name,
			'Popis_sluzby' => $values->desc,
			'Obmedzenia' => $values->rest,
			'Cena_sluzby' => $values->cost,
		));
	}
	
	public function RemoveService($name){	
		$this->database->table("Sluzby")->where('Nazov_sluzby', $name)->update(array('Aktivna' => 0));
		$serv_to_delete = $this->database->table("Sluzby")->where('Nazov_sluzby', $name)->fetch();

		$accomodations = $this->database->table("Ubytovania");
		foreach ($accomodations as $accom) {
			if($accom->Typ_ubytovania == "Rezervacia"){
				foreach ($this->database->table("Ubytovanie_sluzby") as $item) {
					if($item->Ubytovanie_id_ubytovanie == $accom->Ubytovania_id && $item->Sluzby_id_sluzby == $serv_to_delete->Sluzby_id){
						$this->database->table("Ubytovanie_sluzby")
							->where('Ubytovanie_id_ubytovanie', $accom->Ubytovania_id)
							->where('Sluzby_id_sluzby', $serv_to_delete->Sluzby_id)
							->delete();
						$this->database->table('Ubytovania')->update(array('Cena' => ($accom->Cena - ($serv_to_delete->Cena_sluzby*$accom->Dlzka_pobytu))) );
						break;
					}
				}
			}
		}
		
	}
//*************************************************************//
//***************** ROOMS *************************************// 
//*************************************************************//
	public function getTypesRooms(){

		$row = $this->database->table('Typ_izby');

		return $row;			
	}
	public function getRooms($id){

		$works = $this->database->table('Izby')->where('Typ_izby_id_typ', $id)->where('Aktivna', 1);
		return $works;		
	}

	public function AddRoom($values){
		$row=$this->database->table('Izby')->where('Cislo_izby', $values->num)->fetch();
		if(!$row){
		$this->database->table("Izby")->insert(array(
			'Typ_izby_id_typ' => $values->type,
			'Cislo_izby' => $values->num,
			'Poschodie' => $this->get_floor($values->num),
		));
		return 0;
	}
	else return 1;


	}
	public function get_floor($value){
		if(strlen($value)==3)
			$tmp=substr($value, 0, 1);
		elseif(strlen($value)==4)
			$tmp=substr($value, 0, 2);
		else
			$tmp="error";

		return $tmp;
	}
	
	public function getRoomInfo($id){

		$works = $this->database->table('Typ_izby')->where('Typ_izby_id', $id);
		return $works;		
	}

	public function getRoomInfoNum($num){

		$works = $this->database->table('Izby')->where('Cislo_izby', $num);
		return $works;		
	}

	public function ChangePrice($values,$id){

		$this->database->table("Typ_izby")->where('Typ_izby_id', $id)->update(array(
			'Cena_izby' => $values->cost,
		));
	}
	public function RemoveRoom($id){

		$this->database->table("Izby")->where('Cislo_izby', $id)->update(array('Aktivna' => 0));
	}
//*************************************************************//
//***************** WORKER ***********************************// 
//*************************************************************//
	public function getWorkers(){

		$row = $this->database->table('Zamestnanec')->where('Aktivna', 1);
		return $row;			
	}
	public function getWorkerInfo($rc){
		$works = $this->database->table('Zamestnanec')->where('Rodne_cislo', $rc);
		return $works;			
	}
	public function AddWorker($values){
		$row=$this->database->table('Zamestnanec')->where('Rodne_cislo', $values->r_c)->fetch();
		$row2=$this->database->table('Zamestnanec')->where('Login', $values->login)->fetch();
		if(!$row && !$row2){
		$this->database->table("Zamestnanec")->insert(array(
			'Meno' => $values->name,
			'Priezvisko' => $values->sur,
			'Rodne_cislo' => $values->r_c,
			'Pozicia' => $values->pos,
			'Obcianstvo' => $values->obc,
			'Login' => $values->login,
			'Password' => $values->password,
			'Role' => "employee",
		));
	}
	}

	public function ChangeWorker($values, $rc){

		$this->database->table("Zamestnanec")->where('Rodne_cislo',$rc)->update(array(
			'Pozicia' => $values->pos,
		));
	}

	public function RemoveWorker($rc){

		$this->database->table("Zamestnanec")->where('Rodne_cislo', $rc)->update(array('Aktivna' => 0, 'Login' => "" , 'Password' => "" ));
	}
}