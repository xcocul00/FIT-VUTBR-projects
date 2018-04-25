<?php

namespace App\Presenters;

use Nette;
use App\Model\MyProjects;
use Nette\Application\UI\Form;

class MyprojectsPresenter extends Nette\Application\UI\Presenter
{
    private $projId;

    private $projectManager;

    public function __construct(MyProjects $projectManager)
    {
        $this->projectManager = $projectManager;
    }
    
    public function actionDefault()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

    }

    public function actionEdit($id_proj)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->projId = $id_proj;
    }

    public function actionSolved()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }


    public function actionWorker($id_proj)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        $this->projId = $id_proj;
    }


    public function renderDefault()
    {
        $this->template->projects_own = $this->projectManager->getProjInfo($this->getUser()->id);
    }

    public function renderSolved()
    {
        $this->template->projects_own = $this->projectManager->getWorkers($this->getUser()->id);
    }

    public function renderEdit($id_proj)
    {
        $this->template->ID=$id_proj;
        $this->template->info = $this->projectManager->getEditInfo($id_proj);

    
    }

    public function renderWorker($id_proj)
    {
         $this->template->info = $this->projectManager->getEditInfo($id_proj);  
    }

    protected function createComponentButton(){
        $us = $this->projectManager->getUsers($this->getUser()->id);
        $tmp = false;
        $form = new Form;
        $form->addSelect('user_add', '', $us)
             ->setItems($us, FALSE);
        $form->addSubmit('send', 'Pridať riešiteľa');

        $form->onSuccess[] = [$this, 'buttonFormSucceeded'];
        return $form;

    }

    public function buttonFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->newWorker($values, $this->projId);
            $this->flashMessage('Užívateľ bol pridany');
            $this->redirect('Myprojects:worker', $this->projId);

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }

    }

    protected function createComponentLongForm()
    {
        $form = new Form;

        $form->addTextArea('desc', 'Popis:')
            ->setRequired('Prosím vyplnte popis projektu.')
            ->setAttribute('id','dont_resize')
            ->setDefaultValue($this->template->info->Description)
            ->setAttribute('class','max_height')
            ->addRule(Form::MAX_LENGTH, 'Popis moze mat maximalne %d znakov', 50);

        $form->addTextArea('desc_long', 'Zadanie:')
            ->setRequired('Prosím vyplnte konkretnejsi popis projektu.')
            ->setDefaultValue($this->template->info->Text)
            ->setAttribute('id', 'msg')
            ->addRule(Form::MAX_LENGTH, 'Popis moze mat maximalne %d znakov', 5000);
        
        $form->addSubmit('send', 'Zmeniť informácie');

        $form->onSuccess[] = [$this, 'longFormSucceeded'];

        return $form;
    }

    public function longFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->createChange($values, $this->projId, $this->getUser()->getIdentity()->getId());
            $this->flashMessage('Údaje boli zmenené');
            $this->redirect('Myprojects:edit', $this->projId);

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }


}
