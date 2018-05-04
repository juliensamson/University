<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * Medicine
 *
 * @ORM\Table(name="medicine")
 * @ORM\Entity
 */
class Medicine
{
    /**
     * @var string
     *
     * @ORM\Column(name="Barcode", type="string", length=15, nullable=false, options={"comment"="Unique number that manufactor give to its products"})
     */
    private $barcode;

    /**
     * @var string
     *
     * @ORM\Column(name="Description", type="text", length=65535, nullable=false)
     */
    private $description;

    /**
     * @var int
     *
     * @ORM\Column(name="InStock", type="integer", nullable=false)
     */
    private $instock;

    /**
     * @var string
     *
     * @ORM\Column(name="Name", type="string", length=255, nullable=false)
     */
    private $name;

    /**
     * @var int
     *
     * @ORM\Column(name="Quantityinstock", type="integer", nullable=false)
     */
    private $quantityinstock;

    /**
     * @var int
     *
     * @ORM\Column(name="Quantityofpills", type="integer", nullable=false)
     */
    private $quantityofpills;

    /**
     * @var int
     *
     * @ORM\Column(name="MedicineID", type="integer", nullable=false)
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="IDENTITY")
     */
    private $medicineid;

    public function getBarcode(): ?string
    {
        return $this->barcode;
    }

    public function setBarcode(string $barcode): self
    {
        $this->barcode = $barcode;

        return $this;
    }

    public function getDescription(): ?string
    {
        return $this->description;
    }

    public function setDescription(string $description): self
    {
        $this->description = $description;

        return $this;
    }

    public function getInstock(): ?int
    {
        return $this->instock;
    }

    public function setInstock(int $instock): self
    {
        $this->instock = $instock;

        return $this;
    }

    public function getName(): ?string
    {
        return $this->name;
    }

    public function setName(string $name): self
    {
        $this->name = $name;

        return $this;
    }

    public function getQuantityinstock(): ?int
    {
        return $this->quantityinstock;
    }

    public function setQuantityinstock(int $quantityinstock): self
    {
        $this->quantityinstock = $quantityinstock;

        return $this;
    }

    public function getQuantityofpills(): ?int
    {
        return $this->quantityofpills;
    }

    public function setQuantityofpills(int $quantityofpills): self
    {
        $this->quantityofpills = $quantityofpills;

        return $this;
    }

    public function getMedicineid(): ?int
    {
        return $this->medicineid;
    }


}
