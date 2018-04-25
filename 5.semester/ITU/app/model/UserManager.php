<?php

namespace App\Model;

use Nette;
use Nette\Security as NS;

class UserManager{

	use Nette\SmartObject;
	
    private $database;
    private $table = "users";

    function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}

	public function register($data) {

		$row = $this->database->table('users')
			->where('Login', $data->username)->fetch();

		if($row){
			throw new NS\AuthenticationException('This username is already used.');
		}

		$this->database->table($this->table)->insert(array(
			'Login' => $data->username,
			'Password' => $data->password,
			'Name' => $data->name,
			'Surname' => $data->surname,
			'Role' => "user",
		));
	}
}