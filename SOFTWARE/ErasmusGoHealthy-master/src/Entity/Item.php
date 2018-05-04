<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * Item
 *
 * @ORM\Table(name="item", indexes={@ORM\Index(name="FK_Item_Is_taken", columns={"MedicineID"}), @ORM\Index(name="FK_Item_Prescribes", columns={"PresciptionID"})})
 * @ORM\Entity
 */
class Item
{
    /**
     * @var int|null
     *
     * @ORM\Column(name="Liquidquantity", type="integer", nullable=true)
     */
    private $liquidquantity;

    /**
     * @var int|null
     *
     * @ORM\Column(name="Pillsquantity", type="integer", nullable=true)
     */
    private $pillsquantity;

    /**
     * @var int
     *
     * @ORM\Column(name="ItemID", type="integer", nullable=false)
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="IDENTITY")
     */
    private $itemid;

    /**
     * @var int
     *
     * @ORM\Column(name="MedicineID", type="integer", nullable=false)
     */
    private $medicineid;

    /**
     * @var int
     *
     * @ORM\Column(name="PresciptionID", type="integer", nullable=false)
     */
    private $presciptionid;

    public function getLiquidquantity(): ?int
    {
        return $this->liquidquantity;
    }

    public function setLiquidquantity(?int $liquidquantity): self
    {
        $this->liquidquantity = $liquidquantity;

        return $this;
    }

    public function getPillsquantity(): ?int
    {
        return $this->pillsquantity;
    }

    public function setPillsquantity(?int $pillsquantity): self
    {
        $this->pillsquantity = $pillsquantity;

        return $this;
    }

    public function getItemid(): ?int
    {
        return $this->itemid;
    }

    public function getMedicineid(): ?int
    {
        return $this->medicineid;
    }

    public function setMedicineid(int $medicineid): self
    {
        $this->medicineid = $medicineid;

        return $this;
    }

    public function getPresciptionid(): ?int
    {
        return $this->presciptionid;
    }

    public function setPresciptionid(int $presciptionid): self
    {
        $this->presciptionid = $presciptionid;

        return $this;
    }


}
