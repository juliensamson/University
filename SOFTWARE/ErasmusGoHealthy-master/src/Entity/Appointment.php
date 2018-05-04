<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * Appointment
 * 
 * @ORM\Table(name="appointment", indexes={@ORM\Index(name="FK_Appoitment_Creates", columns={"PatientID"}), @ORM\Index(name="FK_Appoitment_Attends", columns={"DoctorID"})})
 * @ORM\Entity(repositoryClass="App\Repository\AppointmentRepository")
 */
class Appointment
{
    /**
     * @var \DateTime
     *
     * @ORM\Column(name="Date", type="datetime", nullable=false, options={"comment"="When the appointment is planned to actually happen"})
     */
    private $date;

    /**
     * @var string|null
     *
     * 
     * @ORM\Column(name="Purpose", type="string", length=255, nullable=true, options={"comment"="Patient describes what is the purpose of appointment. For example, patient got ill or need recommendations."})
     */
    private $purpose;

    /**
     * @var int
     *
     * @ORM\Column(name="AppointmentID", type="integer", nullable=false)
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="IDENTITY")
     */
    private $appointmentid;

    /**
     * @var int
     *
     * @ORM\Column(name="PatientID", type="integer", nullable=false)
     */
    private $patientid;

    /**
     * @var int
     *
     * @ORM\Column(name="DoctorID", type="integer", nullable=false)
     */
    private $doctorid;

    /**
     * @var string
     *
     * @ORM\Column(name="State", type="string", length=50, nullable=false)
     */
    private $state;

    public function getDate(): ?\DateTimeInterface
    {
        return $this->date;
    }

    public function setDate(\DateTimeInterface $date): self
    {
        $this->date = $date;

        return $this;
    }

    public function getPurpose(): ?string
    {
        return $this->purpose;
    }

    public function setPurpose(?string $purpose): self
    {
        $this->purpose = $purpose;

        return $this;
    }

    public function getAppointmentid(): ?int
    {
        return $this->appointmentid;
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

    public function getDoctorid(): ?int
    {
        return $this->doctorid;
    }

    public function setDoctorid(int $doctorid): self
    {
        $this->doctorid = $doctorid;

        return $this;
    }

    public function getState(): ?string
    {
        return $this->state;
    }

    public function setState(string $state): self
    {
        $this->state = $state;

        return $this;
    }


}
