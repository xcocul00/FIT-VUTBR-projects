<?php

namespace App\Model;

use Nette;
use Nette\Security as NS;

class Project{

	use Nette\SmartObject;
	
    private $database;

    function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}


	public function getProjInfo($projId){
		$row = $this->database->table('project')
			->where('ID_project', $projId)->fetch();

		if(! $row){
			throw new NS\AuthenticationException('Unknown poject');
		}

		$owner = $this->database->table('users')
			->where('ID_user', $row->Owner)->fetch();

		$proj_info = array(
			'name' => $row->Name,
			'desc' => $row->Description,
			'text' => $row->Text,
			'public' => $row->Public,
			'date' => $row->Date,
			'owner' => $owner->Login,
			'owner_name' => $owner->Name,
			'owner_surname' => $owner->Surname
		);
		
		return $proj_info;			
	}

	public function getProjCommits($projId){
		$commits = array();

		foreach($this->database->table('info') as $commit){
			if($commit->ID_project == $projId){
				$owner = $this->database->table('users')
					->where('ID_user',$commit->ID_user)
					->fetch();
				array_push($commits, array($commit,$owner));
			}
		}	

		return $commits;
	}

	public function getProjWorkers($projId){
		$solvers = array();

		foreach ($this->database->table('worker') as $worker) {
			if($worker->ID_project == $projId){

				$solver_info = $this->database->table('users')
					->where('ID_user', $worker->ID_user)->fetch();

				array_push($solvers, $solver_info);
			}
		}

		return $solvers;
	}


	public function createProject($values, $userId, $date){

		$this->database->table("project")->insert(array(
			'Name' => $values->name,
			'Description' => $values->desc,
			'Text' => $values->desc_long,
			'Owner' => $userId,
			'Public' => $values->pub,
			'Date' => $date,
		));

		foreach($this->database->table('project') as $key){
			if($key->Name == $values->name){
				$project = $key->ID_project;
			}
		}
		$this->database->table("worker")->insert(array(
			'ID_project' => $project,
			'ID_user' => $userId,
		));


	}

}