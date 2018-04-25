<?php

namespace App\Model;

use Nette;
use Nette\Security as NS;

class Employee{

	use Nette\SmartObject;
	
    private $database;

    function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}

	public function getGuests(){	
		return $this->database->table("Host_hotela");
	}

	public function getGuest($id){
		return $this->database->table("Host_hotela")
			->where('Host_id', $id)->fetch();
	}

	public function getEmployee($id){
		return $this->database->table("Zamestnanec")
			->where('Zamestnanec_id', $id)->fetch();
	}

	public function getInvoice($id){
		return $this->database->table("Vystavene_faktury")
			->where('Faktury_id', $id)->fetch();
	}

	public function getRoomTypes(){
		return $this->database->table("Typ_izby");
	}

	public function getRooms(){
		return $this->database->table("Izby")->where('Aktivna', 1);
	}

	public function getServices(){
		return $this->database->table("Sluzby")->where('Aktivna', 1);
	}

	public function getServices2(){
		return $this->database->table("Sluzby");
	}

	public function getReservation($id){
		return $this->database->table("Ubytovania")->where('Ubytovania_id', $id)->fetch();
	}

	public function getReservations(){
		$reservations = $this->database->table("Ubytovania")->where('Typ_ubytovania','Rezervacia');
		$result = array();

		foreach ($reservations as $reserv) {
			$guest = $this->database->table('Host_hotela')->where('Host_id',$reserv->Host_id_host)->fetch();
			array_push($result, array( $reserv, $guest));

		}

		return $result;
	}

	public function getAccomodations(){
		$accomodations = $this->database->table("Ubytovania")->where('Typ_ubytovania','Ubytovanie');
		$result = array();

		foreach ($accomodations as $reserv) {
			$guest = $this->database->table('Host_hotela')->where('Host_id',$reserv->Host_id_host)->fetch();
			array_push($result, array( $reserv, $guest));

		}

		return $result;
	}

	public function getResRooms($id){
		$all_rooms = $this->database->table("Ubytovanie_izba")->where('Ubytovanie_id_ubytovanie',$id);
		$result = array();

		foreach ($this->database->table("Typ_izby") as $room_type) {
			$type_id = $room_type->Typ_izby_id;
			$rooms_for_type = $this->database->table("Izby")->where('Typ_izby_id_typ',$type_id);
			$my_rooms = array();

			foreach ($rooms_for_type as $room) {
				foreach ($all_rooms as $my_room) {
					if($room->Izby_id == $my_room->Izby_id_izby)
						array_push($my_rooms, $my_room);
				}
			}

			if(empty($my_rooms))
				$result[$type_id] = 0;
			else
				$result[$type_id] = $my_rooms;			
		}

		return $result;
	}

	public function finishFromReservation($id, $user){
		$fac_id = $this->createNewFac();

		$this->database->table('Ubytovania')
			->where('Ubytovania_id', $id)
			->update(array('Typ_ubytovania'=>'Ubytovanie','Zamestnanec_id_zamestnanec' => $user,'Vystavene_faktury_id_faktury' => $fac_id));
	}

	public function getResServices($id){
		$all_services = $this->database->table("Sluzby");
		$reservation_services = $this->database->table("Ubytovanie_sluzby")->where('Ubytovanie_id_ubytovanie',$id);
		$result = array();

		foreach ($all_services as $service) {
			$found = False;
			foreach ($reservation_services as $my_service) {
				if($my_service->Sluzby_id_sluzby == $service->Sluzby_id){
					$found = True;
					break;
				}
			}

			if($found)
				$result[$service->Sluzby_id] = "ano";
			else
				$result[$service->Sluzby_id] = "nie";
		}

		return $result;
	}

	public function reservationChange($values, $resr_rooms, $res_services, $res_info, $free_rooms){
		$rooms_to_delete = array();
		$rooms_to_add = array();
		$service_to_add = array();
		$service_to_delete = array();
		$res_id = $res_info->Ubytovania_id;
		$prize = $res_info->Cena;
		$accom_len = $res_info->Dlzka_pobytu;

		foreach ($values as $key => $value) {
			$type = substr($key,0,1);
			$id = substr($key,1);

			if($type == "R"){

				if($resr_rooms[$id] != 0)
					$old_count = count($resr_rooms[$id]);
				else
					$old_count = 0;
				
				foreach ($free_rooms as $r) {
					if($r['type_id'] == $id){
						$from_rooms = $r['rooms'];
						$prize_change = $r['prize'];
						break;
					}
				}

				if($old_count == $value)
					continue;
				else if($old_count < $value){
					$count = $value - $old_count;

					if(count($from_rooms) < $count){
						return -10;
					}
						
					for ($i=0; $i < $count; $i++) { 
						array_push($rooms_to_add, $from_rooms[$i]->Izby_id);
						$prize += $prize_change*$accom_len;
					}
				}else{
					$count = $old_count - $value;

					for ($i=0; $i < $count; $i++) { 
						array_push($rooms_to_delete, $resr_rooms[$id][$i]->Izby_id_izby);
						$prize -= $prize_change*$accom_len;
					}
				}
			}else if($type == "S"){
				if($value == "ano" && $res_services[$id] == "nie"){
					array_push($service_to_add, $id);
					$prize += $this->database->table('Sluzby')->where('Sluzby_id',$id)->fetch()->Cena_sluzby * $accom_len;
				}else if($value == "nie" && $res_services[$id] == "ano"){
					array_push($service_to_delete, $id);
					$prize -= $this->database->table('Sluzby')->where('Sluzby_id',$id)->fetch()->Cena_sluzby * $accom_len;
				}
			}

		}

		$old_count = 0;
		foreach($resr_rooms as $res){
			if($res != 0)
				$old_count += count($res);
		}
		if($old_count == count($rooms_to_delete) && empty($rooms_to_add)){
			return 1;
		}

		foreach ($rooms_to_add as $room) {
			$this->database->table('Ubytovanie_izba')->insert(array('Izby_id_izby'=>$room, 'Ubytovanie_id_ubytovanie'=>$res_id));
		}

		foreach ($service_to_add as $service) {
			$this->database->table('Ubytovanie_sluzby')->insert(array('Sluzby_id_sluzby'=>$service, 'Ubytovanie_id_ubytovanie'=>$res_id));
		}

		foreach ($rooms_to_delete as $room) {
			$this->database->table('Ubytovanie_izba')
				->where('Izby_id_izby', $room)
				->where('Ubytovanie_id_ubytovanie', $res_id)
				->delete();
		}

		foreach ($service_to_delete as $service) {
			$this->database->table('Ubytovanie_sluzby')
				->where('Sluzby_id_sluzby', $service)
				->where('Ubytovanie_id_ubytovanie', $res_id)
				->delete();
		}

		$this->database->table('Ubytovania')
			->where('Ubytovania_id', $res_id)
			->update(array('Cena'=>$prize,'Komentar'=> $values['Komentar']));
		
		return 0;
	}

	public function cancelReservation($id){
		$this->database->table('Ubytovanie_sluzby')
			->where('Ubytovanie_id_ubytovanie', $id)
			->delete();

		$this->database->table('Ubytovanie_izba')
			->where('Ubytovanie_id_ubytovanie', $id)
			->delete();

		$this->database->table('Ubytovania')
			->where('Ubytovania_id', $id)
			->delete();
	}
	public function createNewFac(){
		$last_fact_num = $this->database->table('Vystavene_faktury')
			->order('Faktury_id DESC')
			->limit(1)
			->fetch()
			->Cislo_faktury;

		$new_num = substr($last_fact_num,2) + 1;
		$fac_num = "AA".$new_num;

		$fac_id = $this->database->table('Vystavene_faktury')->insert(array(
			'Cislo_faktury' => $fac_num
		));

		return $fac_id;
	}

	public function createAccomodation($values, $start_date, $end_date, $guest, $room_info, $all_services, $user){
		$rooms = array();
		$services = array();
		$prize = 0;

		$datediff = strtotime(str_replace('/','-',$end_date)) - strtotime(str_replace('/','-',$start_date));
		$accom_len = ceil($datediff / (60 * 60 * 24)) + 1;

		foreach ($values as $key => $value) {
			$type = substr($key,0,1);
			$id = substr($key,1);
			
			if($type == "R"){
				if($value == 0)
					continue;
				foreach ($room_info as $room) {
					if($room['type_id'] == $id){
						for ($i=0; $i < $value; $i++) { 
							array_push($rooms, $room['rooms'][$i]);
							$prize += $room['prize']*$accom_len;
						}
						break;
					}
				}
			}else if($type == "S"){
				if($value == "ano"){
					foreach ($all_services as $service) {
						if($service->Sluzby_id == $id){
							$prize += $service->Cena_sluzby*$accom_len;
							break;
						}
					}
					array_push($services, $id);
				}
			}
		}

		if(empty($rooms)){
			return 1;
		}

		
		$fac_id = $this->createNewFac();

		$accom_id = $this->database->table('Ubytovania')->insert(array(
			'Cena' => $prize,
			'Sezona' => 'Zima',
			'Typ_ubytovania' => 'Ubytovanie',
			'Zamestnanec_id_zamestnanec' => $user,
			'Host_id_host' => $guest->Host_id,
			'Vystavene_faktury_id_faktury' => $fac_id,
			'date_from' => $start_date,
			'date_to' => $end_date,
			'Dlzka_pobytu' => $accom_len,
			'Komentar' => $values['Komentar']
		));


		foreach ($rooms as $room) {
			$this->database->table('Ubytovanie_izba')->insert(array('Izby_id_izby'=>$room->Izby_id, 'Ubytovanie_id_ubytovanie'=>$accom_id->Ubytovania_id));
		}

		foreach ($services as $serv) {
			$this->database->table('Ubytovanie_sluzby')->insert(array('Sluzby_id_sluzby'=>$serv, 'Ubytovanie_id_ubytovanie'=>$accom_id->Ubytovania_id));
		}
	
		return 0;
	}


}