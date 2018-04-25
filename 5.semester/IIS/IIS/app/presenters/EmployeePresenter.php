<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use App\Model\Employee;
use App\Model\Homepage;
use Nette\Utils\Html;
use App\Model\UserManager;

class EmployeePresenter extends Nette\Application\UI\Presenter
{   
    private $employeeModel;
    private $homepageModel;
    private $userManager;
    private $filters = NULL;
    private $free_rooms = NULL;
    private $reserv_filters = NULL;

    public function __construct( Employee $employeeModel, Homepage $homepageModel, UserManager $userManager)
    {
        $this->employeeModel = $employeeModel;
        $this->homepageModel = $homepageModel;
        $this->userManager = $userManager;
    }

    public function actionDefault()
	{
    	if (!$this->getUser()->isLoggedIn()) {
        	$this->redirect('Sign:in');
    	}

        if (!$this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }

        $this->redirect('Employee:createAccomNew1');
	}


    public function actionAccomodations()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }
    }

    public function filter($arr, $filters){
        $accomodations = $arr;

        if($filters != NULL){
            for ($i=0; $i < count($filters); $i++) { 
                $filter = $filters[$i];
                if(!$filter)
                    continue;

                $tmp = array();
                foreach ($accomodations as $accomodation) {
                    switch ($i) {
                    case 0:
                        if($accomodation[1]->Meno == $filter)   
                            array_push($tmp, $accomodation);
                        break;
                    
                    case 1:
                        if($accomodation[1]->Priezvisko == $filter)   
                            array_push($tmp, $accomodation);
                        break;

                    case 2:
                        if($accomodation[1]->Login == $filter)   
                            array_push($tmp, $accomodation);
                        break;
                    
                    case 3:
                        if( strtotime(str_replace('/','-', $accomodation[0]->date_from)) >= strtotime(str_replace('/','-', $filter)) )
                            array_push($tmp, $accomodation);
                        break;

                    case 4:
                        if( strtotime(str_replace('/','-', $accomodation[0]->date_to)) <= strtotime(str_replace('/','-', $filter)) )
                            array_push($tmp, $accomodation);
                        break;

                    default:
                        break;
                    }
                }
                $accomodations = $tmp;
            }
        }

        return $accomodations;
    }

    public function renderAccomodations()
    {
        $accomodations = $this->employeeModel->getAccomodations();
        $accomodations = $this->filter($accomodations, $this->reserv_filters);

        if(empty($accomodations)){
            $this->flashMessage('Zvolenému filtru nevyhovujú žiadne rezervácie.');
        }

        #TODO -- rozdelenie podla datumu na akutalne a stare
        $this->template->accomodations = $accomodations;
    }

    public function actionAccomodationShow($id)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }

        $this->template->accomodation_info = $this->employeeModel->getReservation($id);
        $this->template->guest = $this->employeeModel->getGuest($this->template->accomodation_info->Host_id_host);
        $this->template->employee = $this->employeeModel->getEmployee($this->template->accomodation_info->Zamestnanec_id_zamestnanec);
        $this->template->invoice = $this->employeeModel->getInvoice($this->template->accomodation_info->Vystavene_faktury_id_faktury);
        $this->template->all_room = $this->employeeModel->getRoomTypes();
        $this->template->accomodation_rooms = $this->employeeModel->getResRooms($id);
        $this->template->accomodation_services = $this->employeeModel->getResServices($id);
        $this->template->services = $this->employeeModel->getServices2();
    }

    public function renderAccomodationShow($id){
       
    }

    public function actionReservations()
    {

        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }
    }

    public function renderReservations()
    {
        $reservations = $this->employeeModel->getReservations();
        $reservations = $this->filter($reservations, $this->reserv_filters);

        if(empty($reservations)){
            $this->flashMessage('Zvolenému filtru nevyhovujú žiadne rezervácie.');
        }

        $this->template->reservations = $reservations;
    }

    protected function createComponentRegisterForm()
    {
        $form = new Form;

        $form->addText('username', 'Uživateľské meno:', 10)
            ->addRule(Form::FILLED, 'Zadajte uživateľské meno')
            ->addRule(Form::MAX_LENGTH, 'Uživateľské meno môže mať maximálne %d znakov.', 10)
            ->setOption('description', 'Meno pod ktorým sa budete prihlasovať do systému')
            ->addCondition(Form::FILLED);

        $form->addPassword('password', 'Heslo:', 10)
            ->addRule(Form::FILLED, 'Prosím vyplnte svoje heslo')
            ->setOption('description', 'Heslo musí mať minimálne 6 znakov')
            ->addRule(Form::MIN_LENGTH, 'Heslo musí mať aspoň %d znakov.', 6)
            ->addRule(Form::MAX_LENGTH, 'Heslo môže mať maximálne %d znakov.', 10);

        $form->addPassword('password2','Heslo znovu:', 10)
            ->addRule(Form::EQUAL, 'Hesla sa nezhodujú.', $form['password'])
            ->addRule(Form::FILLED, 'Zopakujte heslo')
            ->addRule(Form::MAX_LENGTH, 'Heslo môže mať maximálne %d znakov.', 10)
            ->addConditionOn($form['password'], Form::VALID);

        $form->addText('name', 'Meno:', 20)
            ->addRule(Form::FILLED, 'Zadajte vaše meno')
            ->addRule(Form::MAX_LENGTH, 'Meno môže mať maximálne %d znakov.', 20)
            ->addCondition(Form::FILLED);

        $form->addText('surname', 'Priezvisko:', 20)
            ->addRule(Form::FILLED, 'Zadajte vaše priezvisko')
            ->addRule(Form::MAX_LENGTH, 'Meno môže mať maximálne %d znakov.', 20)
            ->addCondition(Form::FILLED);


        $form->addText('email', 'Email:',50)
            ->addRule(Form::FILLED, 'Zadajte email')
            ->addRule(Form::EMAIL, 'Email nemá správny formát')
            ->addRule(Form::MAX_LENGTH, 'E-mail môže mať maximálne %d znakov.', 50);

        $form->addText('cislo_op', 'Číslo dokladu:',20)
            ->addRule(Form::FILLED, 'Zadajte číslo dokladu totožnosti')
            ->addRule(Form::MAX_LENGTH, 'Číslo dokladu môže mať maximálne %d znakov.', 20);

        $form->addSubmit('send', 'Vytvoriť účet')
            ->setAttribute('class', 'register_button');

        $form->onSuccess[] = [$this, 'RegisterFormSucceeded'];

        return $form;
    }

    public function RegisterFormSucceeded($form, $values)
    {
        try {
           $this->userManager->register_guest($values);
           $this->template->msg ='Účet bol úspešne vytvorený.';
        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }

    protected function createComponentReservationFilter(){
        $form = new Form;

        $renderer = $form->getRenderer();
        $renderer->wrappers['controls']['container'] = 'div';
        $renderer->wrappers['pair']['container'] = "div class='control-group'";
        $renderer->wrappers['label']['container'] = "div class='control-label'";
        $renderer->wrappers['control']['container'] = "div class='controls'";

        $form->addText('Meno', 'Meno:');
        $form->addText('Priezvisko', 'Priezvisko:');
        $form->addText('Login', 'Login:');
        $form->addText('Date_from', 'Dátum od:')
            ->setAttribute('id','datepicker_from1')
            ->setAttribute('readonly','true');
        $form->addText('Date_to', 'Dátum do:')
            ->setAttribute('id','datepicker_to1')
            ->setAttribute('readonly','true');
        $form->addSubmit('Send', 'Vyhľadať');

        $form->onSuccess[] = [$this, 'ReservationFilterSucceeded'];

        return $form;
    }

    public function reservationFilterSucceeded($form, $values){
        $this->reserv_filters = array($values->Meno, $values->Priezvisko, $values->Login, $values->Date_from, $values->Date_to);
    }

    public function actionCreateAccomReservation1()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }
    }

    public function renderCreateAccomReservation1()
    {
        $reservations = $this->employeeModel->getReservations();
        $reservations = $this->filter($reservations, $this->reserv_filters);
        

        if(empty($reservations)){
            $this->flashMessage('Zvolenému filtru nevyhovujú žiadne rezervácie.');
        }

        $this->template->reservations = $reservations;
    }

    public function actionCreateAccomReservation2($id)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }
    }

    public function renderCreateAccomReservation2($id)
    {
        $this->template->reservation_info = $this->employeeModel->getReservation($id);
        $this->template->guest = $this->employeeModel->getGuest($this->template->reservation_info->Host_id_host);
        $this->template->all_room = $this->employeeModel->getRoomTypes();
        $this->template->reservation_rooms = $this->employeeModel->getResRooms($id);
        $this->template->reservation_services = $this->employeeModel->getResServices($id);
        $this->template->services = $this->employeeModel->getServices();

    }

    public function actionFinishFromReservation($id){
        
        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }

        try{
            $this->employeeModel->finishFromReservation($id, $this->getUser()->id);
            $this->redirect('Employee:createAccomDone');
        }catch (Exception $e) {
            $form->addError($e->getMessage());
        }
    }

    public function renderFinishFromReservation($id){

    }

    protected function createComponentHostFilter(){
        $form = new Form;

        $renderer = $form->getRenderer();
        $renderer->wrappers['controls']['container'] = 'div';
        $renderer->wrappers['pair']['container'] = "div class='control-group'";
        $renderer->wrappers['label']['container'] = "div class='control-label'";
        $renderer->wrappers['control']['container'] = "div class='controls'";

        $form->addText('Meno', 'Meno:');
        $form->addText('Priezvisko', 'Priezvisko:');
        $form->addText('Login', 'Login:');

        $form->addSubmit('send', 'Vyhladať');

        $form->onSuccess[] = [$this, 'HostFilterSucceeded'];

        return $form;
    }

    public function hostFilterSucceeded($form, $values){

        $this->filters = array($values->Meno, $values->Priezvisko, $values->Login);
    }

    public function actionCreateAccomNew1()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }
    }

    public function renderCreateAccomNew1()
    {   

        $guests = $this->employeeModel->getGuests();

        if($this->filters != NULL){

            if($this->filters[0]){
                $tmp = array();
                foreach ($guests as $guest) {
                    if($guest->Meno == $this->filters[0])
                        array_push($tmp, $guest);
                }
                $guests = $tmp;
            }

            if($this->filters[1]){
                $tmp = array();
                foreach ($guests as $guest) {
                    if($guest->Priezvisko == $this->filters[1])
                        array_push($tmp, $guest);
                }
                $guests = $tmp;
            }

            if($this->filters[2]){
                $tmp = array();
                foreach ($guests as $guest) {
                    if($guest->Login == $this->filters[2])
                        array_push($tmp, $guest);
                }
                $guests = $tmp;
            }
        }

        if(empty($guests)){
            $this->flashMessage('Zvolenému filtru nevyhovujú žiadní hostia.');
        }

        $this->template->guests = $guests;
    }


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
            $this->redirect('Employee:createAccomNew3',$this->template->host_id , $values->from, $values->to);
        }
    }

    public function actionCreateAccomNew2($host_id)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }

        if($host_id == null){
            $this->redirect('Employee:createAccomNew1');
        }

        $this->template->host_id = $host_id;
    }

    public function renderCreateAccomNew2($host_id){
        
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

        $form->addSubmit('send', 'Vytvoriť ubytovanie');

        $form->onSuccess[] = [$this, 'RoomSelectSucceeded'];    
        
        return $form;
    }

    public function RoomSelectSucceeded($form, $values){
        try{
            $res = $this->employeeModel->CreateAccomodation($values, $this->template->start_date, $this->template->end_date, $this->template->guest, $this->template->rooms_info, $this->template->services, $this->getUser()->id);
            if($res != 0){
                $this->flashMessage($res);
            }else{
                $this->redirect('Employee:createAccomDone');
            }
        }catch (Exception $e) {
            $form->addError($e->getMessage());
        }
    }


    public function actionCreateAccomNew3($host_id, $start_date, $end_date)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }

        if($host_id == Null){
            $this->redirect('Employee:createAccomNew1');
        }

        $res = $this->homepageModel->getRoomsInfo(strtotime(str_replace('/','-',$start_date)), strtotime(str_replace('/','-',$end_date)));
        $this->template->rooms_info = $res;
        $this->template->start_date = $start_date;    
        $this->template->end_date = $end_date;
        $this->template->room_types = $this->employeeModel->getRoomTypes();
        $this->template->guest = $this->employeeModel->getGuest($host_id);
        $this->template->services = $this->employeeModel->getServices();
    }

    public function renderCreateAccomNew3($host_id, $start_date, $end_date){

    }

    public function actionCancelReservation($id){
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }

        if(!$id){
            $this->redirect('Empolyee:reservations');
        }

        $this->employeeModel->cancelReservation($id);
    }  

    public function renderCancelReservation($id){
        
    }

    public function actionEditReservation($reservation_id, $msg = NULL)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        if ($this->getUser()->roles[0] != "employee") {
            $this->redirect('Homepage:');
        }

        if(!$reservation_id){
            $this->redirect('Empolyee:reservations');
        }

   
        $this->template->msg = $msg;
       

        $reservation_info = $this->employeeModel->getReservation($reservation_id);
       $guest = $this->employeeModel->getGuest($reservation_info->Host_id_host);

       $reservation_rooms = $this->employeeModel->getResRooms($reservation_id);
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
        $this->template->services = $this->employeeModel->getServices();
        $this->template->reservation_services = $this->employeeModel->getResServices($reservation_id);
    }

    public function renderEditReservation($reservation_id, $msg = NULL){

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
                ->setAttribute('class','comment')
                ->setDefaultValue($this->template->reservation_info->Komentar);

        $form->addSubmit('send', 'Uložiť zmeny');

        $form->onSuccess[] = [$this, 'ResEditSucceeded'];    
        
        return $form;
    }

    public function ResEditSucceeded($form, $values){
        try{   
            $avalible_rooms = $this->homepageModel->getRoomsInfo(strtotime(str_replace('/','-',$this->template->reservation_info->date_from)), strtotime(str_replace('/','-',$this->template->reservation_info->date_to)));
            $res = $this->employeeModel->reservationChange($values, $this->template->reservation_rooms, $this->template->reservation_services, $this->template->reservation_info, $avalible_rooms);
           
            if($res == 1){
                $this->redirect('Employee:EditReservation', $this->template->reservation_info->Ubytovania_id, "V rezervácií musi byť aspoň jedna izba");
            }
            else{
                $this->redirect('Employee:EditReservation', $this->template->reservation_info->Ubytovania_id, "Zmeny boli úspešne uložené");
            }
        }catch (Exception $e) {
            $form->addError($e->getMessage());
        }
    }

}

