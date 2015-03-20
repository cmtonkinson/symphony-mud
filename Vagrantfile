NAME = "symphony"
FQDN = "#{NAME}.example.com"

Vagrant.configure("2") do |config|
  config.vm.box       = "trusty64"
  config.vm.box_url   =
"https://cloud-images.ubuntu.com/vagrant/trusty/current/trusty-server-cloudimg-amd64-vagrant-disk1.box"

  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id,
      # Basics.
      "--name",       NAME,
      "--memory",     1024,
      # I/O APIC must be enabled to support a multi-core guest.
      "--cpus",       4,
      "--ioapic",     "on",
      # Enable native host virtualization features (yields better performance).
      "--pae",        "on",
      "--hwvirtex",   "on",
      "--largepages", "on",
      "--vtxvpid",    "on",
      # This causes the virtual machine to proxy DNS requests through the host
      # via NAT. Without this, the default resolver on the guest will introduce
      # a 5 second latency on every HTTP request... which is a real downer.
      "--natdnshostresolver1", "on"
    ]
  end

  config.vm.hostname = FQDN
  config.vm.provision :shell, path: "bin/provision.sh"
end
