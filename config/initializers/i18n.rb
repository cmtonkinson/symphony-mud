I18n.enforce_available_locales = true
I18n.load_path += Dir[Symphony.root.join 'config', 'locales', '**', '*.yml']
I18n.default_locale = :en
