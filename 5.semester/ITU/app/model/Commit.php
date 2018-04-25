<?php

namespace App\Model;

use Nette;
use Nette\Security as NS;

class Commit{

	use Nette\SmartObject;
	
    private $database;

    function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}

	public function createCommit($values, $projId, $userId){
		
		$this->database->table("info")->insert(array(
			'ID_project' => $projId,
			'ID_user' => $userId,
			'Commit' => $values->desc,
			'Commit_text' => $values->text,
			'Time' => '0000',
			'Date' => $values->date
		));

	}

	public function getCommit($commitId){
		
		$row = $this->database->table('info')
			->where('ID_info', $commitId)->fetch();

		$commit_creator = $this->database->table('users')
			->where('ID_user', $row->ID_user)->fetch();

		$project_name = $this->database->table('project')
			->where('ID_project', $row->ID_project)->fetch();

		$commit = array(
			'desc' => $row->Commit,
			'text' => $row->Commit_text,
			'time' => $row->Time,
			'date' => $row->Date,
			'owner' => $commit_creator->Login,
			'project' => $project_name->Name
		);

		return $commit;
	}

	public function getCommitComents($commitId){
		$coments = array();

		foreach ($this->database->table('coments') as $coment) {
			if($coment->ID_info == $commitId){

				$owner = $this->database->table('users')
					->where('ID_user', $coment->ID_user)->fetch();

				array_push($coments, array(
					'id'  => $coment->ID_coment,
					'date' => $coment->Date,
					'text' => $coment->Text,
					'owner' => $owner->Login
				));
			}
		}

		return $coments;
	}

	public function addComent($values, $commitId, $userId){
		
		$date = getdate();

		$this->database->table("coments")->insert(array(
			'ID_info' => $commitId,
			'ID_user' => $userId,
			'Date' => $date['year'].'-'.$date['mon'].'-'.$date['mday'],
			'Text' => $values->text
		));

	}

}