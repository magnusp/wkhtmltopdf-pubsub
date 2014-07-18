/* -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
 * vi:set ts=4 sts=4 sw=4 noet :
 */

/* This is a simple example program showing how to use the wkhtmltopdf c bindings */
#include <stdbool.h>
#include <stdio.h>
#include <wkhtmltox/pdf.h>

void progress_changed(wkhtmltopdf_converter * c, int p) {
	printf("%3d%%\r",p);
	fflush(stdout);
}

void phase_changed(wkhtmltopdf_converter * c) {
	int phase = wkhtmltopdf_current_phase(c);
	printf("%s\n", wkhtmltopdf_phase_description(c, phase));
}

void error(wkhtmltopdf_converter * c, const char * msg) {
	fprintf(stderr, "Error: %s\n", msg);
}

void warning(wkhtmltopdf_converter * c, const char * msg) {
	fprintf(stderr, "Warning: %s\n", msg);
}

int main() {
	wkhtmltopdf_global_settings * gs;
	wkhtmltopdf_object_settings * os;
	wkhtmltopdf_converter * c;
	wkhtmltopdf_init(false);
	gs = wkhtmltopdf_create_global_settings();
	wkhtmltopdf_set_global_setting(gs, "out", "test.pdf");
	wkhtmltopdf_set_global_setting(gs, "load.cookieJar", "myjar.jar");
	os = wkhtmltopdf_create_object_settings();
	wkhtmltopdf_set_object_setting(os, "page", "http://192.168.3.84");
	c = wkhtmltopdf_create_converter(gs);
	wkhtmltopdf_set_progress_changed_callback(c, progress_changed);
	wkhtmltopdf_set_phase_changed_callback(c, phase_changed);
	wkhtmltopdf_set_error_callback(c, error);
	wkhtmltopdf_set_warning_callback(c, warning);
	wkhtmltopdf_add_object(c, os, NULL);
	if (!wkhtmltopdf_convert(c))
		fprintf(stderr, "Convertion failed!");
	printf("httpErrorCode: %d\n", wkhtmltopdf_http_error_code(c));
	wkhtmltopdf_destroy_converter(c);
	wkhtmltopdf_deinit();
	return 0;
}
