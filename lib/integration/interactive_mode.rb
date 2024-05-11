module Interactive
  class InteractiveMode
    attr_accessor :avatar, :view, :text_buffer, :raster_buffer

    DEFAULT_TEXT_LINES_SHOWN = 5

    def initialize(avatar)
      # Only Avatars can use interactive funcionality.
      @avatar = avatar

      # The Mode and View hold references to one another.
      @view = nil

      # Any non-interactive output gets stored in this circular buffer.
      @text_buffer = []

      # Persistent state for the final "image" we want maintained.
      @raster_buffer = [[]]
    end

    def client
      avatar.user.client
    end

    # How much total horizontal space do we have?
    def output_width
      client.telnet.values[:cols]
    end

    # How much total vertical space do we have?
    def output_height
      client.telnet.values[:rows]
    end

    # Invoked by Universe during output handling - essentially intercepts
    # the avatar.user.client.print call to handle the extra formatting
    # requirements.
    def print(str)
      @text_buffer << str
                      .split("\n")
                      .map { |s| "#{View::InteractiveView::VT100_CLEAR_LINE}#{s}" }
      to_delete = @text_buffer.length - (@view.text_height - 1)
      @text_buffer.shift to_delete if to_delete.positive?
      avatar.user.client.print @view.render_text
    end
  end
end
