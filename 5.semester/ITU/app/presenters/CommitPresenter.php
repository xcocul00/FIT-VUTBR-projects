<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI\Form;
use App\Model\Commit;

class CommitPresenter extends Nette\Application\UI\Presenter
{
    private $projId;
    private $commitManager;
    private $commitId;
    private $presenterId;

    public function __construct(Commit $commitManager)
    {
        $this->commitManager = $commitManager;
    }
    
    public function actionShow($commitId, $projId, $presenterId)
	{
    	if (!$this->getUser()->isLoggedIn()) {
        	$this->redirect('Sign:in');
    	}

        $this->commitId = $commitId;
        $this->projId = $projId;
        $this->presenterId = $presenterId;
	}

    public function actionCreate($projId)
    {
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }

        $this->projId = $projId;
    }

    public function renderShow($commitId, $projId, $presenterId){
         if (!$commitId) {
            $this->error('Stránka nebyla nalezena');
        }

        $commit_info = $this->commitManager->getCommit($commitId);
        $coments = $this->commitManager->getCommitComents($commitId);

        $this->template->commit = $commit_info;
        $this->template->coments = $coments;
        $this->template->projId = $projId;
        $this->template->presenterId = $presenterId;
    }
    
    public function renderCreate($projId){
        if (!$projId) {
            $this->error('Stránka nebyla nalezena');
        }

        $this->template->projId = $projId;
    }

    protected function createComponentComentForm()
    {
        $form = new Form;

        $form->addTextArea('text', '')
            ->setRequired('Zadajte komentar.')
            ->addRule(Form::MAX_LENGTH, 'Popis moze mat maximalne %d znakov',3000);
        
        $form->addSubmit('send', 'Pridať komentár');

        $form->onSuccess[] = [$this, 'comentFormSucceeded'];

        return $form;
    }

    public function comentFormSucceeded($form, $values)
    {
        try {
            $this->commitManager->addComent($values, $this->commitId, $this->getUser()->getIdentity()->getId());
            $this->redirect('Commit:show', $this->commitId ,$this->projId ,$this->presenterId);

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }

    protected function createComponentCommitForm()
    {
        $form = new Form;

        $form->addTextArea('desc', 'Popis:')
            ->setAttribute('id', 'dont_resize')
            ->setAttribute('class','max_height')
            ->setRequired('Prosim vyplnte popis commitu.')
            ->addRule(Form::MAX_LENGTH, 'Popis moze mat maximalne %d znakov', 50);

        $form->addTextArea('text', 'Správa:')
            ->setRequired('Prosím vyplňte spravu.')
            ->setAttribute('id', 'msg')
            ->addRule(Form::MAX_LENGTH, 'Popis moze mat maximalne %d znakov', 2500);

        $form->addText('date','Dátum:')
            ->setRequired('Prosím vyplňte datum YYYY-MM-DD')
            ->setAttribute('class','max_height')
            ->addRule(Form::PATTERN, 'datum musi byt vo formate YYYY-MM-DD', '[2][0][0-1][0-9]-(([0][1-9])|([1][0-2]))-(([0][1-9])|([1-2][0-9])|([3][0-1]))');

        
        $form->addSubmit('send', 'Pridať výkaz');

        $form->onSuccess[] = [$this, 'commitFormSucceeded'];

        return $form;
    }

    public function commitFormSucceeded($form, $values)
    {
        try {
            $this->commitManager->createCommit($values, $this->projId, $this->getUser()->getIdentity()->getId());
            $this->flashMessage('Commit bol pridany');
            $this->redirect('Commit:create', $this->projId);

        } catch (Nette\Security\AuthenticationException $e) {
            $form->addError($e->getMessage());
        }
    }

}
