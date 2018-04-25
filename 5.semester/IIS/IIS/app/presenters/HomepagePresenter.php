<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use App\Model\Homepage;
use App\Model\Employee;

class HomepagePresenter extends Nette\Application\UI\Presenter
{
    private $homepageModel;
    private $employeeModel;

    public function __construct( Homepage $homepageModel, Employee $employeeModel)
    {
        $this->homepageModel = $homepageModel;
        $this->employeeModel = $employeeModel;
    }

    public function actionDefault()
	{
    	if (!$this->getUser()->isLoggedIn()) {
        	$this->redirect('Sign:in');
    	}
	}

    public function actionRooms()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        $this->template->rooms = $this->employeeModel->getRooms();
        $this->template->roomTypes = $this->employeeModel->getRoomTypes();
    }

    public function actionServices()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->template->services = $this->employeeModel->getServices();
    }

    public function actionFreeRooms()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if(!isset($this->template->info)){
            $this->template->info = array();
        }
        
    }

    protected function createComponentFreeForm(){
        $form = new Form;
        
        $form->addText('from')
            ->setAttribute('id', 'date_input')
            ->setRequired()
            ->addRule(Form::PATTERN, 'Datum musi byt v spravnom formate', '(([0][1-9])|([1-2][0-9])|([3][0-1]))/(([0][1-9])|([1][0-2]))/[2][0][0-9][0-9]');

        $form->addText('to')
            ->setAttribute('id', 'date_output')
            ->setRequired()
            ->addRule(Form::PATTERN, 'Datum musi byt v spravnom formate', '(([0][1-9])|([1-2][0-9])|([3][0-1]))/(([0][1-9])|([1][0-2]))/[2][0][0-9][0-9]');

        $form->addSubmit('send', 'Vyhladat');

        $form->onSuccess[] = [$this, 'FreeFormSucceeded'];

        return $form;
    }


    public function freeFormSucceeded($form, $values)
    {
        $start_date = strtotime(str_replace('/','-',$values->from));
        $end_date = strtotime(str_replace('/','-',$values->to));
        
        if($start_date >= $end_date){
            $this->flashMessage('Datum ukoncenia nesmie byt pred alebo rovnaky ako datum zaciatku.');
        }else{
            $res = $this->homepageModel->getRoomsInfo($start_date, $end_date);
            $this->template->info = $res;
            $this->template->start_date = $values->from;    
            $this->template->end_date = $values->to;
        }

    
    }

    public function renderFreeRooms(){
    
    }   

    public function renderDefault()
	{
	}
}
