<?php

namespace App\Model;

use Nette;
use Nette\Security as NS;

class AllProjects{

	use Nette\SmartObject;
	private $database;

    function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}


	public function getProjInfo($own){

		$row = $this->database->table('project')->where('NOT Owner', $own);;

		return $row;			
	}
	public function getProjInfo2($own){

		$row = $this->database->table('project')->where('Owner', $own);

		return $row;			
	}



}