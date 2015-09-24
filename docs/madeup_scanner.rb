# https://github.com/rubychan/coderay/wiki/Scanner-Requests

# Namespace; use this form instead of CodeRay::Scanners to avoid messages like
# "uninitialized constant CodeRay" when testing it.
module CodeRay
module Scanners

  # Always inherit from CodeRay::Scanners::Scanner.
  #
  # Scanner inherits directly from StringScanner, the Ruby class for fast
  # string scanning. Read the documentation to understand what's going on here:
  #
  #   http://www.ruby-doc.org/stdlib/libdoc/strscan/rdoc/index.html
  class Madeup < Scanner

    # Scanners are plugins and must be registered like this:
    register_for :madeup

    # You can provide a file extension associated with this language.
    file_extension 'mup'

    # List all token kinds that are not considered to be running code
    # in this language. For a typical language, this would just be
    # :comment, but for a data or markup language like JSON, no tokens
    # should count as Line of Code.
    KINDS_NOT_LOC = [
      :comment
    ]  # :nodoc:

    # See the WordList documentation.
    CONSTANTS = %w( true false nothing )
    KEYWORDS = %w( repeat for while in to through of by repeatwich if then else end not and or around )
    PREDEFINED_CONSTANTS = %w( pi e )
    PREDEFINED_FUNCTIONS = %w( move moveto yaw pitch roll .radius .innerRadius .outerRadius random sin cos tan asin acos atan atan2 log print debug abs nsides translate rotate scale reframe push pop identity reverse center size min max )
    IDENT_KIND = WordList.new(:key).add(CONSTANTS, :value)
                                   .add(KEYWORDS, :keyword)
                                   .add(PREDEFINED_CONSTANTS, :predefined_constant)
                                   .add(PREDEFINED_FUNCTIONS, :predefined)

    ESCAPE = / [bfnrt\\"\/] /x
    UNICODE_ESCAPE = / u[a-fA-F0-9]{4} /x

    # This is the only method you need to define. It scans code.
    #
    # encoder is an object which encodes tokens. It provides the following API:
    # * encoder.text_token(text, kind) for tokens
    # * encoder.begin_group(kind) and encoder.end_group(kind) for token groups
    # * encoder.begin_line(kind) and encoder.end_line(kind) for line tokens
    #
    # options is a hash. Standard options are:
    # * keep_state: Try to save the current scanner state and restore it in the
    #   next call of scan_tokens.
    #
    # scan_tokens must return the encoder variable it was given.
    #
    # You are completely free to use any style you want, just make sure encoder
    # gets what it needs. But typically, a Scanner follows the following scheme:
    def scan_tokens encoder, options

      # The scanner is always in a certain state, which is :initial by default.
      # We use local variables and symbols to maximize speed.
      state = :initial

      # The main loop; eos? is true when the end of the code is reached.
      until eos?

        case state
          when :initial
            if match = scan(/ \s+ /x)
              encoder.text_token match, :space
            elsif match = scan(/--.*$/)
              encoder.text_token match, :comment
            elsif match = scan(/".*?"/)
              encoder.text_token match, :string
            elsif match = scan(/([-()^+*%|,\[\]:\/<>=!]|\.\.)/)
              encoder.text_token match, :operator
            elsif match = scan(/ true | false | null /x)
              encoder.text_token match, IDENT_KIND[match]
            elsif match = scan(/ -? \d+ \. \d+ /x)
              encoder.text_token match, :float
            elsif match = scan(/ -? \d+ /x)
              encoder.text_token match, :integer
            elsif match = scan(/ ([a-zA-Z]|\.(?!\.))([a-zA-Z0-9]|\.(?!\.))* /x)
              encoder.text_token match, IDENT_KIND[match]
            else
              encoder.text_token getch, :error
            end
          end
      end

      encoder
    end

  end

end
end
