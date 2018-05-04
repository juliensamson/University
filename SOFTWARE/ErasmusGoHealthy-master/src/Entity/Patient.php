<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * Patient
 * 
 * @ORM\Table(name="patient", indexes={@ORM\Index(name="FK_Patient_Can_be", columns={"PersonID"})})
 * @ORM\Entity(repositoryClass="App\Repository\PatientRepository")
 */
class Patient
{
    /**
     * @var string
     *
     * @ORM\Column(name="PersonalId", type="string", length=15, nullable=false, options={"comment"="Unique number that each citizen have."})
     */
    private $personalid;

    /**
     * @var int
     *
     * @ORM\Column(name="PatientID", type="integer", nullable=false)
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="IDENTITY")
     */
    private $patientid;

    /**
     * @var int
     *
     * @ORM\Column(name="PersonID", type="integer", nullable=false)
     */
    private $personid;

    public function getPersonalid(): ?string
    {
        return $this->personalid;
    }

    public function setPersonalid(string $personalid): self
    {
        $this->personalid = $personalid;

        return $this;
    }

    public function getPatientid(): ?int
    {
        return $this->patientid;
    }

    public function getPersonid(): ?int
    {
        return $this->personid;
    }

    public function setPersonid(int $personid): self
    {
        $this->personid = $personid;

        return $this;
    }


}
