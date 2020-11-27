module Command
  ###############################################################################
  define :preferences do
    usage
    usage({ preference: %w[color numeric] }, { toggle: :bool })
    exec do
      # Helper!
      def yn(bool)
        bool ? '{GYES{x' : '{RNO{x'
      end

      case preference
      when nil
        being.puts 'nil'
        being.puts t('title')
        being.puts t('color'), yn(being.pref_show_color?)
        being.puts t('numeric'), yn(being.pref_show_numeric?)
      when 'color'
        toggle ? being.pref_show_color! : being.pref_hide_color!
        being.puts t('changed_bool'), 'color output', yn(being.pref_show_color?)
      when 'numeric'
        toggle ? being.pref_show_numeric! : being.pref_hide_numeric!
        being.puts t('changed_bool'), 'damage numbers', yn(being.pref_show_numeric?)
      else
        being.puts 'else'
        being.puts t('error')
      end
      next
    end
  end

  ###############################################################################
  define :quit do
    usage
    exec do
      being.cmd 'save'
      being.puts t('actor')
      being.user.client.should_terminate = true
    end
  end

  ###############################################################################
  define :save do
    usage
    exec do
      being.print t('start')
      Logistics::Scribe.save being
      being.puts t('success')
    end
  end

  ###############################################################################
  define :whoami do
    usage
    exec do
      being.puts t('actor'), being
    end
  end
end
