NAME = "symphony"
FQDN = "#{NAME}.example.com"

Vagrant.configure("2") do |config|
  config.vm.box       = "trusty32"
  config.vm.box_url   = "https://cloud-images.ubuntu.com/vagrant/trusty/current/trusty-server-cloudimg-i386-vagrant-disk1.box"

  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id,
      # Basics.
      "--name",       NAME,
      "--memory",     2048,
      # I/O APIC must be enabled to support a multi-core guest.
      "--cpus",       4,
      "--ioapic",     "on",
      # Enable native host virtualization features (yields better performance).
      "--pae",        "on",
      "--hwvirtex",   "on",
      "--largepages", "on",
      "--vtxvpid",    "on"
    ]
  end

  config.vm.hostname = FQDN
  config.vm.provision :shell, path: "provision.sh"
end
