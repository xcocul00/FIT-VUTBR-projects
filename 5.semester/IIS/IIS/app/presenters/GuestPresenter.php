<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use App\Model\Guest;
use App\Model\Homepage;
use Nette\Utils\Html;
use App\Model\UserManager;

class GuestPresenter extends Nette\Application\UI\Presenter
{
    private $projectManager;
    private $homepageModel;
    private $reserv_filters = NULL;

    public function __construct(Guest $projectManager, Homepage $homepageModel)
    {
        $this->projectManager = $projectManager;
        $this->homepageModel = $homepageModel;
    }
    public function actionDefault()
	{
    	if (!$this->getUser()->isLoggedIn()) {
        	$this->redirect('Sign:in');
    	}

        $this->redirect('Guest:accom');
	}

        public function actionAccom()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }

        public function actionReservations()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }

        public function actionCreateReservation()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }
        public function actionEditReservation($reservation_id,$msg = NULL)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');       
        }
        if(!$reservation_id){
            $this->redirect('Guest:reservations');
        }
        $this->template->msg=$msg;
        $reservation_info = $this->projectManager->getReservation($reservation_id);
        $guest = $this->projectManager->getGuest($reservation_info->Host_id_host);
        
        $reservation_rooms = $this->projectManager->getResRooms($reservation_id);
        $rooms_info = $this->homepageModel->getRoomsInfo(strtotime(str_replace('/','-',$reservation_info->date_from)), strtotime(str_replace('/','-',$reservation_info->date_to)));


        $tmp = array();
        foreach ($rooms_info as $info) {
            if($reservation_rooms[$info['type_id']] != 0){
                foreach ($reservation_rooms[$info['type_id']] as $room) {
                   array_push($info['rooms'], $room);
                }
            }
            array_push($tmp, $info);
        }

        $rooms_info = $tmp;

        $this->template->rooms_info = $rooms_info;
        $this->template->reservation_info = $reservation_info;
        $this->template->guest = $guest;
        $this->template->reservation_rooms = $reservation_rooms;
        $this->template->services = $this->projectManager->getServices();
        $this->template->reservation_services = $this->projectManager->getResServices($reservation_id);
    }
        public function actionCancelReservation($id)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if(!$id){
            $this->redirect('Empolyee:reservations');
        }

        $this->projectManager->cancelReservation($id);
    }  

/****************************************************************/
/*************** MOJE IZBY **************************************/
/****************************************************************/
    public function renderAccom()
    {
        $accomodations = $this->projectManager->getAccomodations($this->getUser()->id);
        $accomodations = $this->projectManager->filter($accomodations, $this->reserv_filters);
        if(empty($accomodations)){
            $this->flashMessage('Zvolenemu filtru nevyhovuju ziadne rezervacie.');
        }

        $this->template->accomodations = $accomodations;
    }
    
    protected function createComponentReservationFilter(){
        $form = new Form;

        $renderer = $form->getRenderer();
        $renderer->wrappers['controls']['container'] = 'div';
        $renderer->wrappers['pair']['container'] = "div class='control-group'";
        $renderer->wrappers['label']['container'] = "div class='control-label'";
        $renderer->wrappers['control']['container'] = "div class='controls'";

        $form->addText('Date_from', 'Dátum od:')
            ->setAttribute('id','datepicker_from1')
            ->setAttribute('readonly','true');
        $form->addText('Date_to', 'Dátum do:')
            ->setAttribute('id','datepicker_to1')
            ->setAttribute('readonly','true');
        $form->addSubmit('Send', 'Vyhladať');

        $form->onSuccess[] = [$this, 'ReservationFilterSucceeded'];

        return $form;
    }

    public function reservationFilterSucceeded($form, $values){
        $this->reserv_filters = array($values->Date_from, $values->Date_to);
    }

    public function actionAccomShow($id)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->template->accomodation_info = $this->projectManager->getReservation($id);
        $this->template->guest = $this->projectManager->getGuest($this->template->accomodation_info->Host_id_host);
        $this->template->employee = $this->projectManager->getEmployee($this->template->accomodation_info->Zamestnanec_id_zamestnanec);
        $this->template->all_room = $this->projectManager->getRoomTypes();
        $this->template->accomodation_rooms = $this->projectManager->getResRooms($id);
        $this->template->accomodation_services = $this->projectManager->getResServices($id);
        $this->template->services = $this->projectManager->getServices2();
        $this->template->invoice = $this->projectManager->getInvoice($this->template->accomodation_info->Vystavene_faktury_id_faktury);
    }
/****************************************************************/
/*************** Rezervacia **************************************/
/****************************************************************/
    protected function createComponentGetFreeRooms(){
        $form = new Form;
        
        $form->addText('from')
            ->setAttribute('id', 'date_input')
            ->setAttribute('readonly', 'readonly')
            ->setRequired();     

        $form->addText('to')
            ->setAttribute('id', 'date_output')
            ->setAttribute('readonly', 'readonly')
            ->setRequired();

        $form->addSubmit('send', 'Vyhladať');

        $form->onSuccess[] = [$this, 'GetFreeRoomsSucceeded'];

        return $form;
    }

    public function getFreeRoomsSucceeded($form, $values)
    {
        $start_date = strtotime(str_replace('/','-',$values->from));
        $end_date = strtotime(str_replace('/','-',$values->to));
        
        if($start_date >= $end_date){
            $this->flashMessage('Dátum ukončenia nesmie byť pred, alebo rovnaký ako dátum začiatku.');
        }else{
            $this->redirect('Guest:createReservation2',$this->getUser()->id , $values->from, $values->to);
        }
    }

    public function actionCreateReservation2($host_id, $start_date, $end_date)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        
        $res = $this->homepageModel->getRoomsInfo(strtotime(str_replace('/','-',$start_date)), strtotime(str_replace('/','-',$end_date)));

        $this->template->start_date = $start_date;    
        $this->template->end_date = $end_date;
        $this->template->guest = $this->projectManager->getGuest($host_id);
        $this->template->rooms_info = $res;
        $this->template->services = $this->projectManager->getServices();
    }

    protected function createComponentRoomSelect(){

        $form = new Form;

        
        $renderer = $form->getRenderer();
        $renderer->wrappers['controls']['container'] = 'div';
        $renderer->wrappers['pair']['container'] = "div class='control-group'";
        $renderer->wrappers['label']['container'] = "div class='control-label'";
        $renderer->wrappers['control']['container'] = "div class='controls'";


        $form->addGroup(1)
            ->setOption("container",Html::el('div class="rooms"')->setHtml("span"));

        $room_count = count($this->template->rooms_info);
        foreach ($this->template->rooms_info as $room_type) {
            $count = array("0" => "0");
            $counter = 1;
            foreach ($room_type['rooms'] as $room) {
                array_push($count, $counter);
                $counter++;
            }
            $room_count--;
            
            if($room_count == 0){
                $form->addSelect('R'.$room_type['type_id'],'',$count)
                    ->setAttribute('id','last_room')
                    ->setRequired(TRUE);
            }else{
                $form->addSelect('R'.$room_type['type_id'],'',$count)
                    ->setRequired();
            }
            
        }

        $form->addGroup(2)
            ->setOption("container",Html::el('div class="serv"')->setHtml("span"));

        foreach ($this->template->services as $service) {
            $form->addRadioList('S'.$service->Sluzby_id, '', ['ano'=>'ano', 'nie'=>'nie'])
                ->getSeparatorPrototype()->setName(NULL)
                ->setRequired(TRUE)
                ->setDefaultValue('0');
        }

        $form->addGroup(3)
            ->setOption("container",Html::el('div class="comm"')->setHtml("span"));

        $form->addTextArea('Komentar','')
                ->setAttribute('class','comment');

        $form->addSubmit('send', 'Vytvoriť rezerváciu');

        $form->onSuccess[] = [$this, 'RoomSelectSucceeded'];    
        
        return $form;
    }

    public function RoomSelectSucceeded($form, $values){
        try{
            $res = $this->projectManager->createReservation($values, $this->template->start_date, $this->template->end_date, $this->template->guest, $this->template->rooms_info, $this->template->services, $this->getUser()->id);
            if($res != 0){
                $this->flashMessage($res);
            }else{
                $this->redirect('Guest:createReservationDone');
            }
        }catch (Exception $e) {
            $form->addError($e->getMessage());
        }
    }
/****************************************************************/
/*************** MOJE REZERVACIE ********************************/
/****************************************************************/
    public function renderReservations()
    {
        $reservations = $this->projectManager->getReservations($this->getUser()->id);
        $reservations = $this->projectManager->filter($reservations, $this->reserv_filters);

        if(empty($reservations)){
            $this->flashMessage('Zvolenému filtru nevyhovujú žiadne rezervácie.');
        }

        $this->template->reservations = $reservations;
    }
    protected function createComponentReservationFilter2(){
        $form = new Form;

        $renderer = $form->getRenderer();
        $renderer->wrappers['controls']['container'] = 'div';
        $renderer->wrappers['pair']['container'] = "div class='control-group'";
        $renderer->wrappers['label']['container'] = "div class='control-label'";
        $renderer->wrappers['control']['container'] = "div class='controls'";

        $form->addText('Date_to', 'Dátum do:')
            ->setAttribute('id','datepicker_to1')
            ->setAttribute('readonly','true');
        $form->addSubmit('Send', 'Vyhladať');

        $form->onSuccess[] = [$this, 'ReservationFilter2Succeeded'];

        return $form;
    }

    public function reservationFilter2Succeeded($form, $values){
        $this->reserv_filters = array($values->Date_from, $values->Date_to);
    }
    public function renderEditReservation($reservation_id,$msg = NULL){


    }
protected function createComponentResEdit(){

        $form = new Form;

        
        $renderer = $form->getRenderer();
        $renderer->wrappers['controls']['container'] = 'div';
        $renderer->wrappers['pair']['container'] = "div class='control-group'";
        $renderer->wrappers['label']['container'] = "div class='control-label'";
        $renderer->wrappers['control']['container'] = "div class='controls'";
        
        $form->addGroup(1)
            ->setOption("container",Html::el('div class="rooms"')->setHtml("span"));

        $room_count = count($this->template->rooms_info);
        foreach ($this->template->rooms_info as $room_type) {
            $count = array("0" => "0");
            $counter = 1;
            foreach ($room_type['rooms'] as $room) {
                array_push($count, $counter);
                $counter++;
            }
            $room_count--;

            if($this->template->reservation_rooms[$room_type['type_id']] == 0)
                $def_select = 0;
            else
                $def_select = count($this->template->reservation_rooms[$room_type['type_id']]);

            if($room_count == 0){
                $form->addSelect('R'.$room_type['type_id'],'',$count)
                    ->setAttribute('id','last_room')
                    ->setDefaultValue($def_select)
                    ->setRequired(TRUE);
            }else{
                $form->addSelect('R'.$room_type['type_id'],'',$count)
                    ->setDefaultValue($def_select)
                    ->setRequired();
            }
            
        }

        $form->addGroup(2)
            ->setOption("container",Html::el('div class="serv"')->setHtml("span"));

        
        foreach ($this->template->services as $service) {
            if($this->template->reservation_services[$service->Sluzby_id] == "ano")
                $def_service = "ano";
            else
                $def_service = "nie";

            $form->addRadioList('S'.$service->Sluzby_id, '', ['ano'=>'ano', 'nie'=>'nie'])
                ->setDefaultValue($def_service)
                ->getSeparatorPrototype()->setName(NULL)
                ->setRequired(TRUE)
                ->setDefaultValue('0');
        }

        $form->addGroup(3)
            ->setOption("container",Html::el('div class="comm"')->setHtml("span"));

        $form->addTextArea('Komentar','')
                ->setAttribute('class','comment');

        $form->addSubmit('send', 'Uložiť zmeny');

        $form->onSuccess[] = [$this, 'ResEditSucceeded'];    
        
        return $form;
    }

    public function ResEditSucceeded($form, $values){
        try{   
            $avalible_rooms = $this->homepageModel->getRoomsInfo(strtotime(str_replace('/','-',$this->template->reservation_info->date_from)), strtotime(str_replace('/','-',$this->template->reservation_info->date_to)));
            $res = $this->projectManager->reservationChange($values, $this->template->reservation_rooms, $this->template->reservation_services, $this->template->reservation_info, $avalible_rooms);
           
            if($res == 1){
                $this->redirect('Guest:EditReservation', $this->template->reservation_info->Ubytovania_id, "V rezervácií musí byť aspoň jedna izba");
            }
            else{
                $this->redirect('Guest:EditReservation', $this->template->reservation_info->Ubytovania_id, "Zmeny boli úspešne uložené");
            }
        }catch (Exception $e) {
            $form->addError($e->getMessage());
        }
    }
}
