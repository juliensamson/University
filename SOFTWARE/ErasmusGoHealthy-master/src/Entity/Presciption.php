<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * Presciption
 *
 * @ORM\Table(name="presciption", indexes={@ORM\Index(name="FK_Presciption_Creates", columns={"DoctorID"}), @ORM\Index(name="FK_Presciption_Can_get", columns={"PatientID"})})
 * @ORM\Entity
 */
class Presciption
{
    /**
     * @var \DateTime
     *
     * @ORM\Column(name="CreationDate", type="datetime", nullable=false)
     */
    private $creationdate;

    /**
     * @var \DateTime
     *
     * @ORM\Column(name="EndDate", type="datetime", nullable=false)
     */
    private $enddate;

    /**
     * @var string
     *
     * @ORM\Column(name="Instruction", type="string", length=255, nullable=false)
     */
    private $instruction;

    /**
     * @var int
     *
     * @ORM\Column(name="PresciptionID", type="integer", nullable=false)
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="IDENTITY")
     */
    private $presciptionid;

    /**
     * @var int
     *
     * @ORM\Column(name="DoctorID", type="integer", nullable=false)
     */
    private $doctorid;

    /**
     * @var int
     *
     * @ORM\Column(name="PatientID", type="integer", nullable=false)
     */
    private $patientid;

    public function getCreationdate(): ?\DateTimeInterface
    {
        return $this->creationdate;
    }

    public function setCreationdate(\DateTimeInterface $creationdate): self
    {
        $this->creationdate = $creationdate;

        return $this;
    }

    public function getEnddate(): ?\DateTimeInterface
    {
        return $this->enddate;
    }

    public function setEnddate(\DateTimeInterface $enddate): self
    {
        $this->enddate = $enddate;

        return $this;
    }

    public function getInstruction(): ?string
    {
        return $this->instruction;
    }

    public function setInstruction(string $instruction): self
    {
        $this->instruction = $instruction;

        return $this;
    }

    public function getPresciptionid(): ?int
    {
        return $this->presciptionid;
    }

    public function getDoctorid(): ?int
    {
        return $this->doctorid;
    }

    public function setDoctorid(int $doctorid): self
    {
        $this->doctorid = $doctorid;

        return $this;
    }

    public function getPatientid(): ?int
    {
        return $this->patientid;
    }

    public function setPatientid(int $patientid): self
    {
        $this->patientid = $patientid;

        return $this;
    }


}
