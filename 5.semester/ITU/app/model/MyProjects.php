<?php

namespace App\Model;

use Nette;
use Nette\Security as NS;


class MyProjects{

	use Nette\SmartObject;
	private $database;

    function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}

	public function getProjInfo($own){

		$row = $this->database->table('project')->where('Owner', $own);

		return $row;			
	}

	public function getWorkers($id_user){

		$pole=array();
		$works = $this->database->table('worker')->where('ID_user', $id_user);
		foreach ($works as $key) {
			$row = $this->database->table('project')->where('ID_project', $key->ID_project)->fetch();
			array_push($pole, $row);
		}
		return $pole;
	}

	public function getEditInfo($id_proj){
		$info= $this->database->table('project')->where('ID_project', $id_proj)->fetch();
		return $info;
	}

	public function getUsers($id_user){
		$pole=array();
		$works = $this->database->table('users')->where('Role', 'user')->where('NOT ID_user', $id_user);
		foreach ($works as $key) {
			$login=$key->Login;
			array_push($pole, $login);
		}
		return $pole;
	}
	
	public function createChange($values, $projId, $userId){
		
		$this->database->table("project")->where('ID_project',$projId)->update(array(
			'Description' => $values->desc,
			'Text' => $values->desc_long,
		));

	}

	public function newWorker($values, $proj_id){
		$works = $this->database->table('users')->where('Login', $values->user_add);
		$this->database->table("worker")->insert(array(
			'ID_project' => $proj_id,
			'ID_user' => $works,
		));	
		

	}


}