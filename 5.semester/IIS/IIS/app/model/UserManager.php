<?php

namespace App\Model;

use Nette;
use Nette\Security as NS;

class UserManager{

	use Nette\SmartObject;
	
    private $database;
    private $table = "Host_hotela";

    function __construct(Nette\Database\Context $database){
		$this->database = $database;
	}

	public function register_guest($data) {

		$row = $this->database->table('Host_hotela')
			->where('Login', $data->username)->fetch();

		if($row){
			throw new NS\AuthenticationException('Uzivatel s takymto menom uz existuje.');
		}

		$row = $this->database->table('Zamestnanec')
			->where('Login', $data->username)->fetch();

		if($row){
			throw new NS\AuthenticationException('Uzivatel s takymto menom uz existuje.');
		}

		$this->database->table($this->table)->insert(array(
			'Login' => $data->username,
			'Password' => $data->password,
			'Meno' => $data->name,
			'Priezvisko' => $data->surname,
			'Cislo_dokladu_totoznosti' => $data->cislo_op,
			'email' => $data->email,
			'Role' => "guest",
		));

	}
}