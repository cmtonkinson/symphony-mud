# This file contains the View::InteractiveView class, which is responsible for
# rendering interactive screens in-game. The view is divided into two
# zones: the raster zone (where dynamic content is drawn) and and the text
# zone (where normal I/O is displayed but where normal prompts and newlines
# are squelched for the sake of real estate. The text zone is alwasy exactly
# DEFAULT_TEXT_LINES_SHOWN lines tall, and the raster zone takes up the rest of
# the screen.

# NOTE: For all coordinate calculations per VT100 the origin (1,1) is the top-
# left cell of the display, with X increasing downward and Y increasing right-
# ward.
module View
  class InteractiveView
    DEFAULT_TEXT_LINES_SHOWN = 5
    VT100_CLEAR_DISPLAY = "\e[2J".freeze
    VT100_CLEAR_LINE = "\e[2K".freeze

    def initialize(mode)
      @mode = mode
      mode.view = self
    end

    # VT100 escape sequence for moving the cursor to a specific position.
    def vt100_cursor_position(col, row)
      "\e[#{col};#{row}H"
    end

    # How many cells tall is the text zone?
    def text_height
      DEFAULT_TEXT_LINES_SHOWN
    end

    # How many cells wide is the text zone?
    def text_width
      @mode.output_width
    end

    # Where is the top-left cell of the text zone?
    def text_origin
      # The 2 is to account for the borders.
      [raster_height + 2, 1]
    end

    # How many cells tall is the raster zone?
    def raster_height
      # The minus two is for the borders.
      @mode.output_height - text_height - 2
    end

    # How many cells wide is the raster zone?
    def raster_width
      @mode.output_width
    end

    # Return the width and height of the raster zone.
    def raster_dimensions
      [raster_width, raster_height]
    end

    # Where is the top-left cell of the raster zone?
    def raster_origin
      [2, 1]
    end

    def render
      @mode.avatar.print VT100_CLEAR_DISPLAY
      @mode.avatar.print render_top_border
      @mode.avatar.print render_raster
      @mode.avatar.print render_middle_border
      @mode.avatar.print render_text
    end

    def render_top_border
      "#{vt100_cursor_position(1, 1)}[------- E to exit -------]"
    end

    def render_middle_border
      vt100_cursor_position(raster_height + 1, 1) + ('-' * (text_width - 1))
    end

    def render_text
      '%s%s%s%s' % [
        vt100_cursor_position(*text_origin),
        @mode.text_buffer.join("\n"),
        "\n",
        VT100_CLEAR_LINE
      ]
    end

    def render_raster
      "#{vt100_cursor_position(*raster_origin)}_render_raster"
    end
  end
end
