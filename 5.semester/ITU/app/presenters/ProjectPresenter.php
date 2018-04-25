<?php

namespace App\Presenters;

use Nette;
use App\Model\Project;
use Nette\Application\UI\Form;

class ProjectPresenter extends Nette\Application\UI\Presenter
{
    private $projectManager;

    public function __construct(Project $projectManager)
    {
        $this->projectManager = $projectManager;
    }

    public function actionCreate()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }

    public function actionShow($projId)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

         if (!$projId) {
            $this->error('Stránka nebyla nalezena');
        }
    }

    public function actionEdit()
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }

    public function actionShowcommits($projId)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
    }

    public function renderShowcommits($projId){

        if (!$projId) {
            $this->error('Stránka nebyla nalezena');
        }

        $this->template->projId = $projId;
        $this->template->commits = $this->projectManager->getProjCommits($projId);
    }

    public function renderShow($projId){

        try{
            $proj_info = $this->projectManager->getProjInfo($projId);
            $proj_commits = $this->projectManager->getProjCommits($projId);
            $proj_workers = $this->projectManager->getProjWorkers($projId);

            $this->template->workers = array();

            foreach ($proj_workers as $worker) {
                
                $worker_commits = array();
                foreach($proj_commits as $commit){
                    if($commit[0]->ID_user == $worker->ID_user){
                        array_push($worker_commits, $commit[0]);
                    }
                }

                array_push($this->template->workers, array(
                    'login' => $worker->Login,
                    'name' => $worker->Name,
                    'surname' => $worker->Surname,
                    'commits' => $worker_commits
                ));
            }

            $this->template->projId = $projId;
            $this->template->proj_info = $proj_info;

        }catch (Nette\Security\AuthenticationException $e){
            $this->error($e->getMessage());
        }

    }

    protected function createComponentCreateForm()
    {
        $form = new Form;
        $public = ['1'=>'áno','0'=>'nie'];

        $form->addTextArea('name', 'Názov:')
            ->setRequired('Prosím vyplnte názov projektu.')
            ->setAttribute('id','name')
            ->setAttribute('class','max_height')
            ->addRule(Form::MAX_LENGTH, 'Popis moze mat maximalne %d znakov', 50);
            
        $form->addTextArea('desc', 'Popis:')
            ->setRequired('Prosím vyplnte popis projektu.')
            ->setAttribute('id','dont_resize')
            ->setAttribute('class','max_height')
            ->addRule(Form::MAX_LENGTH, 'Popis moze mat maximalne %d znakov', 50);

        $form->addTextArea('desc_long', 'Zadanie:')
            ->setRequired('Prosím vyplnte popis projektu.')
            ->setAttribute('id', 'msg')
            ->addRule(Form::MAX_LENGTH, 'Popis moze mat maximalne %d znakov', 5000);

        $form->addRadioList('pub', 'Verejný:', $public)
             ->setDefaultValue('1');
        
        $form->addSubmit('send', 'Vytvoriť projekt');

        $form->onSuccess[] = [$this, 'createFormSucceeded'];

        return $form;
    }

    public function createFormSucceeded($form, $values)
    {
        try {
            $this->projectManager->createProject($values, $this->getUser()->getIdentity()->getId(), date("Y/m/d"));
            $this->flashMessage('Projekt bol pridany');
            $this->redirect('Project:create');

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }

}
