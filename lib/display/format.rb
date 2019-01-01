module Display
  class Format
    ###########################################################################
    def self.quantity(value, hash)
      found = hash.find { |x,y| value >= x }
      found ? found[1] : "[{RERROR{x - value (#{value}) outside of range for Display::Format::quantity]"
    end

    ###########################################################################
    def self.health(percentage)
      quantity(percentage, {
        0.98 => 'looks {Gfantastic{x',
        0.90 => 'is in {Bexcellent{x condition',
        0.70 => 'is in {Mgreat{x condition',
        0.50 => 'is in {Cgood{x condition',
        0.35 => 'looks a little {cworn{x',
        0.20 => 'looks pretty {mworn{x',
        0.10 => 'looks pretty {ybattered{x',
        0.05 => 'is very {rbeat up{x',
        0.01 => 'looks {wterrible{x',
        0.00 => 'is on the {wbrink of death{x',
      })
    end

    ###########################################################################
    def self.damage(amount)
      quantity(amount, {
        2000 => 'does {?S{?O{?U{?L {?C{?R{?U{?S{?H{?I{?N{?G{x things to',
        1000 => '{WOBLITERATES{x',
        750  => '{RRUINS{x',
        500  => '{YTORTURES{x',
        250  => '{w--{W=={r{{{R{{{WANNIHILATES{R}{r}{W=={w--{x',
        200  => '{b=={c**{CDEVASTATES{c**{b=={x',
        180  => '{w<{x<{W<{RDESOLATES{W>{x>{w>{x',
        160  => '{g({G({YSHATTERS{G){g){x',
        140  => '{W[{BBREAKS{W]{x',
        120  => '{YDISFIGURES{x',
        100  => '{BDESTROYS{x',
        80   => '{RSCARS{x',
        70   => '{Gdecimates{x',
        60   => '{Rmaims{x',
        50   => '{Ymutilates{x',
        40   => '{Bmangles{x',
        35   => '{Mbatters{x',
        30   => '{Cdamages{x',
        25   => '{winjures{x',
        20   => '{charms{x',
        16   => '{yhurts{x',
        12   => '{gwounds{x',
        8    => '{bbruises{x',
        4    => '{rscratches{x',
        1    => '{mgrazes{x',
        0    => 'does nothing to',
      })
    end

    ###########################################################################
    def self.mastery(percent)
      quantity(percent, {
        100 => '{Wmasterful{x',
        95  => '{Gsuperb{x',
        85  => '{Cexcellent{x',
        75  => '{Mvery good{x',
        60  => '{ggood{x',
        41  => '{cokay{x',
        31  => '{minferior{x',
        15  => '{rpoor{x',
        0   => '{wbad{x'
      })
    end

    ###########################################################################
    def self.height(number)
      quantity(number, {
        5 => 'huge',
        4 => 'tall',
        3 => 'average',
        2 => 'short',
        1 => 'tiny',
      })
    end

    ###########################################################################
    def self.weight(number)
      quantity(number, {
        5 => 'obese',
        4 => 'heavy',
        3 => 'average',
        2 => 'thin',
        1 => 'bony',
      })
    end

  end
end
