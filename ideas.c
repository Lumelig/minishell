
int shell_loop(t_env *env)
{
    char *input;
    token_t *tokens;
    cmd_t *commands;

    while (1) {
        // Handle signals first
        if (g_sigint_received) {
            handle_signal_interrupt(env);
            continue;
        }

        // Get user input
        input = readline("minishell$ ");
        if (!input) {  // EOF (Ctrl+D)
            printf("exit\n");
            break;
        }

        // Skip empty input
        if (!*input) {
            free(input);
            continue;
        }

        // Add to history and process
        add_history(input);
        tokens = tokenize(input);
        commands = parse(tokens);
        
        if (commands && validate_syntax(tokens) == SYNTAX_OK) {
            execute_pipeline(commands, env);
        }

        // Cleanup
        cleanup_resources(input, tokens, commands);
    }

    return (env->exit_status);
}
void handle_signal_interrupt(t_env *env)
{
    // Reset the signal flag
    g_sigint_received = 0;
    
    // Set exit status based on shell state
    if (env->state == STATE_EXECUTING) {
        env->exit_status = 130;  // Standard Ctrl+C during command execution
    } else {
        env->exit_status = 1;    // Ctrl+C at prompt (bash behavior)
    }
    
    // Clean up readline interface
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    
    // Reset shell state to interactive
    env->state = STATE_INTERACTIVE;
    
    // If we were in middle of something, clean it up
    if (env->current_input) {
        free(env->current_input);
        env->current_input = NULL;
    }
    
    // Reset any heredoc state
    if (env->in_heredoc) {
        cleanup_heredoc(env);
        env->in_heredoc = 0;
    }
}
syntax_error_t validate_basic_syntax(token_t *tokens)
{
    token_t *current = tokens;
    token_t *prev = NULL;
    
    // Check structural syntax errors that don't depend on expansion
    while (current && current->type != TOKEN_EOF) {
        
        // 1. Pipe at start
        if (!prev && current->type == TOKEN_PIPE) {
            printf("minishell: syntax error near unexpected token '|'\n");
            return SYNTAX_ERROR_PIPE_AT_START;
        }
        
        // 2. Consecutive operators
        if (prev && is_operator(prev->type) && is_operator(current->type)) {
            printf("minishell: syntax error near unexpected token '%s'\n", 
                   current->value);
            return SYNTAX_ERROR_CONSECUTIVE_OPERATORS;
        }
        
        // 3. Pipe at end
        if (current->type == TOKEN_PIPE) {
            if (!current->next || current->next->type == TOKEN_EOF) {
                printf("minishell: syntax error: unexpected end of input\n");
                return SYNTAX_ERROR_PIPE_AT_END;
            }
        }
        
        // 4. Redirection without filename token
        if (is_redirect_operator(current->type)) {
            if (!current->next || current->next->type == TOKEN_EOF) {
                printf("minishell: syntax error near unexpected token 'newline'\n");
                return SYNTAX_ERROR_REDIRECT_NO_FILE;
            }
            current = current->next;  // Skip filename token
        }
        
        prev = current;
        current = current->next;
    }
    
    return SYNTAX_OK;
}