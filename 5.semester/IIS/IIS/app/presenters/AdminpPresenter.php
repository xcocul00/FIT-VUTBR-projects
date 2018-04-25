<?php

namespace App\Presenters;

use Nette;
use App\Model\Admin;
use Nette\Application\UI\Form;

class AdminpPresenter extends Nette\Application\UI\Presenter
{
    
    private $projectManager;
    private $rc;
    private $id;
    private $room_number;
    private $service;

    public function __construct(Admin $projectManager)
    {
        $this->projectManager = $projectManager;
    }
    public function actionDefault()
	{
    	if (!$this->getUser()->isLoggedIn()) {
        	$this->redirect('Sign:in');
    	}

        $this->redirect('Adminp:rooms');
	}

    public function actionRooms()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }

    public function actionServices()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }
    public function actionWorkers()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }
     public function actionChangeService($service)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->service=$service;
    }
    public function actionRemoveService($service)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->service=$service;
    }

    public function actionCreateService()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }

    public function actionCreateRoom()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }
    
    public function actionChangePrice($id)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->id=$id;
    }
    
    public function actionRemoveRoom($room_number)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->room_number=$room_number;
    }  
    public function actionCreateWorker()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    } 
    public function actionChangeWorker($rc)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->rc=$rc;
    } 
    public function actionRemoveWorker($rc)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->rc=$rc;
    } 
//*************************************************************//
//***************** SERVICE ***********************************// 
//*************************************************************//  
    public function renderServices()
    {
         $this->template->services = $this->projectManager->getServices();
    }

    public function renderChangeService($service)
    {
          $this->template->service=$this->projectManager->getServiceInfo($service);
    }

    public function renderRemoveService($service)
    {
         $this->template->service=$this->projectManager->getServiceInfo($service);
    }

    protected function createComponentAdd_service(){

        $form = new Form;
        
        $form->addText('name', 'Názov služby')
            ->setRequired('Prosím vyplnte názov služby.')
            ->addRule(Form::MAX_LENGTH, 'Popis môže mať maximálne %d znakov', 30);
        $form->addText('desc', 'Popis služby');
        $form->addText('rest', 'Obmedzenie služby');
        $form->addText('cost', 'Cena')
            ->setRequired('Prosím vyplnte cenu služby.')
            ->addRule(Form::FLOAT, 'Zadajte číselnú hodnotu, celé čísla oddelte znakom ","')
            ->addRule(Form::MAX_LENGTH, 'Popis môže mať maximálne %d znakov', 10);
        $form->addSubmit('send', 'Pridať službu');

        $form->onSuccess[] = [$this, 'Add_serviceFormSucceeded'];
        return $form;
    }

    public function Add_serviceFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->AddService($values);
            $this->flashMessage('Údaje boli pridané');
            $this->redirect('Adminp:Services');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }
    protected function createComponentChange_service(){

        $form = new Form;
        $form->addText('name', 'Názov služby')
            ->setRequired('Prosím vyplnte názov služby.')
            ->addRule(Form::MAX_LENGTH, 'Popis môže mať maximálne %d znakov', 30);
        $form->addText('desc', 'Popis služby');
        $form->addText('rest', 'Obmedzenie služby');
        $form->addText('cost', 'Cena')
            ->setRequired('Prosím vyplnte cenu služby.')
            ->addRule(Form::FLOAT, 'Zadajte číselnú hodnotu, celé čísla oddelte znakom ","')
            ->addRule(Form::MAX_LENGTH, 'Popis môže mať maximálne %d znakov', 10);
        $form->addSubmit('send', 'Zmeniť službu');

        $form->onSuccess[] = [$this, 'Change_serviceFormSucceeded'];
        return $form;
    }

    public function Change_serviceFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->ChangeService($values,$this->service);
            $this->flashMessage('Údaje boli zmenene');
            $this->redirect('Adminp:Services');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }
    protected function createComponentRemove_service(){

        $form = new Form;
        $form->addSubmit('send', 'Zmazať');

        $form->onSuccess[] = [$this, 'Remove_serviceFormSucceeded'];
        return $form;
    }

    public function Remove_serviceFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->RemoveService($this->service);
            $this->flashMessage('Údaje boli zmazané');
            $this->redirect('Adminp:services');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }
//*************************************************************//
//***************** ROOMS *************************************// 
//*************************************************************//

    public function renderRooms()
    {
         //$this->template->rooms = $this->projectManager->getRooms();
         $types=$this->projectManager->getTypesRooms();
         $this->template->type=$types;
         $this->template->test = array();
         foreach ($types as $key) {
            $this->template->test[$key->Typ_izby_id] = $this->projectManager->getRooms($key->Typ_izby_id);
         }
    }

    public function renderCreateRoom()
    {
         $this->template->type=$this->projectManager->getTypesRooms();
    }

    public function renderChangePrice($id)
    {
         $this->template->room=$this->projectManager->getRoomInfo($id);
    }

    public function renderRemoveRoom($room_number)
    {
         $this->template->room=$this->projectManager->getRoomInfoNum($room_number);
    }   
    protected function createComponentAdd_room(){

        $form = new Form;
        $form->addText('type', 'ID izby')
            ->setRequired('Prosím vyplnte typ izby.')
            ->setOption('description', 'Použite ID typu izby z tabuľky Typy izieb hore')
            ->addRule(Form::MAX_LENGTH, 'Typ môže mat maximalne %d znakov', 11);
        $form->addText('num', 'Číslo izby')
            ->setRequired('Prosím vyplnte číslo izby.')
            ->addRule(Form::MIN_LENGTH, 'Izba je v rozsahu 100-9999', 3)
            ->addRule(Form::MAX_LENGTH, 'Popis môže mať maximálne %d znakov', 4);
        $form->addSubmit('send', 'Pridať izbu');

        $form->onSuccess[] = [$this, 'Add_roomFormSucceeded'];
        return $form;
    }

    public function Add_roomFormSucceeded($form, $values)
    {
        try {
            if(($this->projectManager->Addroom($values))==0){
            $this->flashMessage('Údaje boli pridané');
        }
        else
            $this->flashMessage('Izba už existuje');
            $this->redirect('Adminp:rooms');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }  

    protected function createComponentChange_price(){

        $form = new Form;
        $form->addText('cost', 'Nová cena izby')
            ->setRequired('Prosím vyplnte cenu izby.')
            ->addRule(Form::FLOAT, 'Zadajte číselnu hodnotu, celé čísla oddelte ","')
            ->addRule(Form::MAX_LENGTH, 'Typ môže mat maximalne %d znakov', 11);
        $form->addSubmit('send', 'Zmeniť cenu');

        $form->onSuccess[] = [$this, 'Change_priceFormSucceeded'];
        return $form;
    }

    public function Change_priceFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->ChangePrice($values,$this->id);
            $this->flashMessage('Údaje boli zmenené');
            $this->redirect('Adminp:rooms');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }   

    protected function createComponentRemove_room(){

        $form = new Form;
        $form->addSubmit('send', 'Zmazať');

        $form->onSuccess[] = [$this, 'Remove_roomFormSucceeded'];
        return $form;
    }

    public function Remove_roomFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->RemoveRoom($this->room_number);
            $this->flashMessage('Údaje boli zmazané');
            $this->redirect('Adminp:rooms');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }
//*************************************************************//
//***************** WORKER ***********************************// 
//*************************************************************//
    public function renderWorkers()
    {
         $this->template->workers = $this->projectManager->getWorkers();
    }
    protected function createComponentAdd_worker(){

        $form = new Form;
        $form->addText('name', 'Meno')
            ->setRequired('Prosím vyplnte meno zamestnanca.')
            ->addRule(Form::MAX_LENGTH, 'Meno môže mat maximalne %d znakov', 20);
        $form->addText('sur', 'Priezvisko')
            ->setRequired('Prosím vyplnte priezvisko zamestnanca.')
            ->addRule(Form::MAX_LENGTH, 'Priezvisko môže mať maximálne %d znakov', 20);
        $form->addText('r_c', 'Rodne cislo')
            ->setOption('description', 'Rodne číslo zadajte bez znaku "/"' )
            ->setRequired('Prosím vyplnte rodné číslo zamestnanca.')
            ->addRule(Form::MAX_LENGTH, 'Číslo môže mať maximálne %d znakov', 10);
        $form->addText('pos', 'Pozicia')
            ->setRequired('Prosím vyplnte pozíciu zamestnanca.')
            ->addRule(Form::MAX_LENGTH, 'Popis môže mať maximálne %d znakov', 20);
         $form->addText('obc', 'Obcianstvo')
            ->setRequired('Prosím vyplnte občianstvo zamestnanca.')
            ->setOption('description', 'Použite skratku štátu')
            ->addRule(Form::MAX_LENGTH, 'Občianstvo môže mať maximálne %d znakov', 20);
        $form->addText('login', 'Prihlasovacie meno')
            ->setRequired('Prosím vyplnte login zamestnanca.')
            ->addRule(Form::MAX_LENGTH, 'Popis môže mať maximálne %d znakov', 10); 
        $form->addText('password', 'Heslo')
            ->setRequired('Prosím vyplnte heslo zamestnanca.')
            ->addRule(Form::MAX_LENGTH, 'Heslo môže mať maximálne %d znakov', 10);          
        $form->addSubmit('send', 'Pridať');

        $form->onSuccess[] = [$this, 'Add_workerFormSucceeded'];
        return $form;
    }

    public function Add_workerFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->AddWorker($values);
            $this->flashMessage('Údaje boli pridané');
            $this->redirect('Adminp:Workers');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }
    public function renderChangeWorker($rc)
    {
         $this->template->worker = $this->projectManager->getWorkerInfo($rc);
    }

    protected function createComponentChange_worker(){

        $form = new Form;
        $form->addText('pos', 'Pozícia')
            ->setRequired('Prosím vyplnte názov pozície.')
            ->setOption('description', 'Zadajte novú pozíciu')
            ->addRule(Form::MAX_LENGTH, 'Popis môže mať maximálne %d znakov', 20);
        $form->addSubmit('send', 'Zmeniť poziciu');

        $form->onSuccess[] = [$this, 'Change_workerFormSucceeded'];
        return $form;
    }

    public function Change_workerFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->ChangeWorker($values,$this->rc);
            $this->flashMessage('Údaje boli zmenené');
            $this->redirect('Adminp:Workers');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }
    public function renderRemoveWorker($rc)
    {
         $this->template->worker = $this->projectManager->getWorkerInfo($rc);
    }

    protected function createComponentRemove_worker(){

        $form = new Form;
        $form->addSubmit('send', 'Odstrániť');

        $form->onSuccess[] = [$this, 'Remove_workerFormSucceeded'];
        return $form;
    }

    public function Remove_workerFormSucceeded($form)
    {
        try {
            $this->projectManager->RemoveWorker($this->rc);
            $this->flashMessage('Údaje boli zmazané');
            $this->redirect('Adminp:Workers');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }
}
