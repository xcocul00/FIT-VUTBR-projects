<?php

namespace App\Model;

use Nette;
use Nette\Security as NS;

class Homepage{

	use Nette\SmartObject;
	
    private $database;

    function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}


	public function getRoomsInfo($start_date, $end_date){
		$occupied_rooms = array();
		

		foreach($this->database->table('Ubytovania') as $row){

			if((strtotime(str_replace('/','-',$row->date_from)) >= $end_date) || (strtotime(str_replace('/','-',$row->date_to)) <= $start_date)){
				continue;
			}else{

				$rooms_rows = $this->database->table('Ubytovanie_izba')
					->where('Ubytovanie_id_ubytovanie', $row->Ubytovania_id);

				foreach($rooms_rows as $room_row){
					array_push($occupied_rooms, $room_row->Izby_id_izby);
				}
			}
		}
		
		$result = array();
		foreach ($this->database->table('Typ_izby') as $row) {
			$free_rooms = array();

			$room_rows = $this->database->table('Izby')
				->where('Typ_izby_id_typ', $row->Typ_izby_id)
				->where('Aktivna', 1);

		    foreach ($room_rows as $room) {
		    	if( !in_array($room->Izby_id, $occupied_rooms) ){
		    		array_push($free_rooms, $room);
		    	}
		    }

		    array_push($result, array(
		    	"type_id" => $row->Typ_izby_id,
		    	"desc" => $row->Typ_izby,
		    	"prize" => $row->Cena_izby,
		    	"rooms" => $free_rooms
		    ));
		}

		return $result;		
	}

}