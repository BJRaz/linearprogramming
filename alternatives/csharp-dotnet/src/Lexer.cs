using System;
using System.Collections.Generic;
using System.Text;

namespace LPSolver
{
    public class Lexer
    {
        private readonly string _input;
        private int _position = 0;
        private int _line = 1;
        private int _column = 1;
        private List<string> _errors = new();

        public Lexer(string input)
        {
            _input = input;
        }

        public List<Token> Tokenize()
        {
            var tokens = new List<Token>();

            while (_position < _input.Length)
            {
                // Skip whitespace
                if (char.IsWhiteSpace(CurrentChar))
                {
                    if (CurrentChar == '\n')
                    {
                        _line++;
                        _column = 1;
                    }
                    else
                    {
                        _column++;
                    }
                    _position++;
                    continue;
                }

                // Skip comments
                if (IsLineComment() || IsBlockComment())
                    continue;

                Token? token = NextToken();
                if (token != null)
                {
                    tokens.Add(token);
                }
            }

            tokens.Add(new Token(TokenType.EOF, "", _line, _column));
            return tokens;
        }

        private Token? NextToken()
        {
            var startLine = _line;
            var startColumn = _column;

            return CurrentChar switch
            {
                '+' => Advance(TokenType.Plus, "+"),
                '-' => Advance(TokenType.Minus, "-"),
                '<' => Advance(TokenType.Less, "<"),
                '>' => Advance(TokenType.Greater, ">"),
                '=' => Advance(TokenType.Equal, "="),
                ';' => Advance(TokenType.Semicolon, ";"),
                _ when char.IsDigit(CurrentChar) => ReadNumber(),
                _ when char.IsLetter(CurrentChar) || CurrentChar == '_' => ReadIdentifier(),
                _ => HandleUnexpectedChar(startLine, startColumn)
            };
        }

        private Token ReadNumber()
        {
            var startLine = _line;
            var startColumn = _column;
            var sb = new StringBuilder();

            while (_position < _input.Length && 
                   (char.IsDigit(CurrentChar) || CurrentChar == '.'))
            {
                sb.Append(CurrentChar);
                _column++;
                _position++;
            }

            return new Token(TokenType.Number, sb.ToString(), startLine, startColumn);
        }

        private Token ReadIdentifier()
        {
            var startLine = _line;
            var startColumn = _column;
            var sb = new StringBuilder();

            while (_position < _input.Length && 
                   (char.IsLetterOrDigit(CurrentChar) || CurrentChar == '_'))
            {
                sb.Append(CurrentChar);
                _column++;
                _position++;
            }

            return new Token(TokenType.Identifier, sb.ToString(), startLine, startColumn);
        }

        private bool IsLineComment()
        {
            if (_position + 1 < _input.Length && 
                _input[_position] == '/' && 
                _input[_position + 1] == '/')
            {
                // Skip until end of line
                while (_position < _input.Length && _input[_position] != '\n')
                    _position++;
                return true;
            }
            return false;
        }

        private bool IsBlockComment()
        {
            if (_position + 1 < _input.Length && 
                _input[_position] == '/' && 
                _input[_position + 1] == '*')
            {
                _position += 2;
                // Skip until */
                while (_position + 1 < _input.Length)
                {
                    if (_input[_position] == '*' && _input[_position + 1] == '/')
                    {
                        _position += 2;
                        break;
                    }
                    if (_input[_position] == '\n') _line++;
                    _position++;
                }
                return true;
            }
            return false;
        }

        private Token Advance(TokenType type, string value)
        {
            var token = new Token(type, value, _line, _column);
            _position++;
            _column++;
            return token;
        }

        private Token? HandleUnexpectedChar(int line, int column)
        {
            _errors.Add($"Unexpected character '{CurrentChar}' at {line}:{column}");
            _position++;
            _column++;
            return null;
        }

        private char CurrentChar => 
            _position < _input.Length ? _input[_position] : '\0';

        public List<string> Errors => _errors;
    }
}
